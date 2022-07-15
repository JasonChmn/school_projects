#include "pointcontrole.h"

PointControle::PointControle()
{
    coords.x = coords.y = coords.z = 0;
    hauteur = 4;
    largeur = 2;
    selectionne = false;
}

PointControle::PointControle(float x, float y, float z)
{
    coords.x = x;
    coords.y = y;
    coords.z = z;
    hauteur = 4;
    largeur = 2;
    selectionne = false;
}

void PointControle::setSelectionne(bool etat)
{
    selectionne = etat;
}

void PointControle::setCoords(float x, float y, float z)
{
    coords.x = x;
    coords.y = y;
    coords.z = z;
}

void PointControle::moveCoords(float x, float y, float z)
{
    coords.x += x;
    coords.y += y;
    coords.z += z;
}
Vecteur3D PointControle::getCoords()
{
    return coords;
}

void PointControle::drawPC()
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,0,hauteur);
        glVertex3f(largeur,largeur,0);
        glVertex3f(-largeur,largeur,0);
        glVertex3f(-largeur,-largeur,0);
        glVertex3f(largeur,-largeur,0);
        glVertex3f(largeur,largeur,0);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,0,-hauteur);
        glVertex3f(largeur,largeur,0);
        glVertex3f(-largeur,largeur,0);
        glVertex3f(-largeur,-largeur,0);
        glVertex3f(largeur,-largeur,0);
        glVertex3f(largeur,largeur,0);
    glEnd();
}


void PointControle::UpdateDraw()
{
    //std::cout << "dessine PC" << std::endl;
    glPushMatrix();
    if (selectionne)
    {
        glColor3f(0.5,0.5,0.5);
    }
    else
    {
        glColor3f(0.2,0.2,1);
    }
    drawPC();

    glPopMatrix();
}

void PointControle::UpdateName()
{
    glPushMatrix();
    //J'update le reste
    drawPC();
    if(!m_children.empty())
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(m_children[i] != NULL)
            {
                m_children[i]->UpdateName();
            }
        }
    }
    glPopMatrix();
}
