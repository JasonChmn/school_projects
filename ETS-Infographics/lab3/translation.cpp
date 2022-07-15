#include "translation.h"

Translation::Translation()
{
    x = y = z = 0;
    m_Parent = NULL;
    m_Name = NULL;
}

Translation::Translation(float newX, float newY, float newZ, Node *Parent, const char *Name)
{
    x = newX;
    y = newY;
    z = newZ;

    m_Parent = Parent;
    m_Name = Name;
}

void Translation::UpdateDraw()
{
    glPushMatrix();
    glTranslatef(x,y,z);
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

void Translation::UpdateName()
{
    glPushMatrix();
    glTranslatef(x,y,z);
    //J'update le reste
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

void Translation::setTranslation(float newX, float newY, float newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}

void Translation::moveTranslation(float movex, float movey, float movez)
{
    x += movex;
    y += movey;
    z += movez;
}

float* Translation::getPtX()
{
    return &x;
}

float* Translation::getPtY()
{
    return &y;
}

float* Translation::getPtZ()
{
    return &z;
}
