#include "vaisseau.h"

#define PI (3.141592653589793)

Vaisseau::Vaisseau()
{
    mon_vaisseau = glmReadOBJ("vaisseau.obj");
    bouclierOn = false;
    OrbiteBouclier = new Orbite(false,0,0,0);
    OrbiteBouclier->setAlpha(0.5);
    OrbiteBouclier->setRGB(0,0.3,0.5);
}

void Vaisseau::drawVaisseau()
{
    glPushMatrix();
    if (bouclierOn)
    {
        drawBouclier();
    }
    glColor3f(0.9,0.9,0.9);
    glEnable(GL_NORMALIZE);
    glmDraw(mon_vaisseau, GLM_MATERIAL | GLM_SMOOTH);
    glPopMatrix();
}

void Vaisseau::UpdateDraw()
{
    glPushMatrix();

    //std::cout << "je dessine mon vaisseau"<<std::endl;
    //Je dessine ma planète
    drawVaisseau();
    //J'update le reste
    if(!m_children.empty())
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(m_children[i] != NULL)
            {
                m_children[i]->UpdateDraw();
            }
        }
    }
    glPopMatrix();
}

void Vaisseau::UpdateName()
{
    NULL;
}

void Vaisseau::drawBouclier()
{
    float sommet = 23; // coord Z du sommet
    OrbiteBouclier->setHauteurAnneau(0.2);
    float base = 8;   // coord X ou Y à la base
    OrbiteBouclier->setLargeurAnneau(base/10);

    int k;
    std::cout<<"Je dessine le bouclier " << std::endl;
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glRotatef(90,1,0,0);
    for (k=1;k<5;k++)
    {
        glPushMatrix();
            glTranslatef(0,0,-(sommet-(float)(5-k)*2));
            OrbiteBouclier->setRayon(base-(float)k*2);
            OrbiteBouclier->drawOrbite();
        glPopMatrix();
    }
    glPopMatrix();
    glPopAttrib();
}

void Vaisseau::setBouclier(bool etat)
{
    bouclierOn = etat;
}
