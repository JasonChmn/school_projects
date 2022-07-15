#include "axexyz.h"

AxeXYZ::AxeXYZ()
{
    hauteur = 500;
    largeur = 0.5;
    largeur2 = 5;
}

void AxeXYZ::UpdateDraw(void)
{
    glPushMatrix();
    if (modeXYZ)
    {
        glPushAttrib(GL_LIGHTING);
        glDisable(GL_LIGHTING);
        glPushMatrix();
            // axe X en rouge
            glColor3f(1,0,0);
            drawAxe();
            glRotatef(90,0,0,1);
            // axe Y en vert
            glColor3f(0,1,0);
            drawAxe();
            glRotatef(-90,0,1,0);
            // axe Z en bleu
            glColor3f(0,0,1);
            drawAxe();
        glPopMatrix();
        glPopAttrib();
    }

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

void AxeXYZ::UpdateName(void)
{
    NULL;
}

void AxeXYZ::drawAxe()
{
    glBegin(GL_QUADS);
        glVertex3f(-hauteur,-largeur,0);
        glVertex3f(hauteur,-largeur,0);
        glVertex3f(hauteur,largeur,0);
        glVertex3f(-hauteur,largeur,0);

        glVertex3f(-hauteur,0,-largeur);
        glVertex3f(hauteur,0,-largeur);
        glVertex3f(hauteur,0,largeur);
        glVertex3f(-hauteur,0,largeur);
    glEnd();

    glColor3f(0.8,0.6,0.8);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(hauteur+(int)(hauteur/40),0,0);
        glVertex3f(hauteur,largeur2,-largeur2);
        glVertex3f(hauteur,largeur2,largeur2);
        glVertex3f(hauteur,-largeur2,largeur2);
        glVertex3f(hauteur,-largeur2,-largeur2);
    glEnd();
}
