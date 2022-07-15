#include "courbe.h"

Courbe::Courbe()
{
}



Vecteur3D Courbe::getVecteurActuel()
{
    return vecteurActuel;
}

Vecteur3D Courbe::getPosActuelle()
{
    return posActuelle;
}


void Courbe::UpdateDraw(void)
{
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
} // Update()

/*****************************************************************************/

void Courbe::UpdateName(void)
{
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
} // Update()
