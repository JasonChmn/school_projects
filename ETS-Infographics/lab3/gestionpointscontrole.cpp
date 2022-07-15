#include "gestionpointscontrole.h"

GestionPointsControle::GestionPointsControle()
{
    tabPoints.clear();
}

/***********************************************************************/

int GestionPointsControle::getNbPoints()
{
    return tabPoints.size();
}

/***********************************************************************/

std::vector<PointControle*> GestionPointsControle::getPoints()
{
    return tabPoints;
}

/***********************************************************************/

void GestionPointsControle::addPoint(float x, float y, float z)
{
    PointControle* point = new PointControle(x,y,z);
    Translation* translation = new Translation(x,y,z);

    // J'ajoute dans le vecteur
    tabPoints.push_back(point);
    tabTranslation.push_back(translation);

    // J'ajoute dans le graphe de scene
    translation->AddChildNode(point);
    this->AddChildNode(translation);
}

/***********************************************************************/

void GestionPointsControle::removePoint(int numero)
{
    // Je desalloue la memoire
    delete tabPoints[numero];
    delete tabTranslation[numero];

    this->RemoveChildNode(tabTranslation[numero]);

    // Je supprime du vecteur
    tabPoints.erase(tabPoints.begin() + numero);
    tabTranslation.erase(tabTranslation.begin() + numero);
}

/***********************************************************************/

void GestionPointsControle::changeControlePoint(int numero, float x, float y, float z)
{
    //std::cout << "nouvelle coords xyz : "<< x << " , " << y << std::endl;
    tabPoints[numero]->moveCoords(x,y,z);
    tabTranslation[numero]->moveTranslation(x,y,z);
}

/***********************************************************************/

void GestionPointsControle::selection(int numero,bool etat)
{
    tabPoints[numero]->setSelectionne(etat);
}

/***********************************************************************/

Vecteur3D GestionPointsControle::getCoord(int numero)
{
    return tabPoints[numero]->getCoords();
}

/***********************************************************************/
/***********************************************************************/

void GestionPointsControle::UpdateDraw()
{
    //std::cout <<"nombre PC : "<<m_children.size()<<std::endl;
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);

    glPushMatrix();

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

    glPopAttrib();
}

void GestionPointsControle::UpdateName()
{
    glPushMatrix();
    //J'update le reste
    if(!m_children.empty())
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            //std::cout << "je fais mon name : "<<i<<std::endl;
            glPushName(i+1);
            if(m_children[i] != NULL)
            {
                m_children[i]->UpdateName();
            }
            glPopName();
        }
    }
    glPopMatrix();
}

