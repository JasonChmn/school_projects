#include "rotation.h"

Rotation::Rotation(float newAngle, int new_ex, int new_ey, int new_ez,
                   Node *Parent, const char *Name)
{
    angle = newAngle;
    ex = new_ex;
    ey = new_ey;
    ez = new_ez;

    m_Parent = Parent;
    m_Name = Name;
}

Rotation::Rotation(Node* node)
{
    m_Parent = node->GetParentNode();
    m_Name = node->GetNodeName();
}

void Rotation::UpdateDraw()
{
    glPushMatrix();
    glRotatef(angle,ex,ey,ez);
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

void Rotation::UpdateName()
{
    glPushMatrix();
    glRotatef(angle,0,0,0);
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

void Rotation::setAngle(float newAngle)
{
    angle = newAngle;
}

void Rotation::setAxeRotation(int new_ex, int new_ey, int new_ez)
{
    ex = new_ex;
    ey = new_ey;
    ez = new_ez;
}

float* Rotation::getPtAngleR()
{
    return &angle;
}
