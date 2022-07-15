#include "lignedroite.h"

LigneDroite::LigneDroite()
{
    timeCourbe = 0;
}


void LigneDroite::addtime(float newTime)
{
    //std::cout << "timeCourbe "<<timeCourbe << std::endl;
    updateLigne();
    if (!marcheArriere)
    {
        timeCourbe += newTime;
        if (timeCourbe*vitesseParticule/2 > distanceTotal)
        {
            std::cout << "RAZ" << std::endl;
            timeCourbe = 0;
        }
    }
    else
    {
        timeCourbe -= newTime;
        if (timeCourbe<0)
        {
            std::cout << "RAZ pour arriere" << std::endl;
            timeCourbe = distanceTotal*2/vitesseParticule;
            posActuelle = tabPoints[tabPoints.size()-1]->getCoords();
        }
    }
}



void LigneDroite::init(std::vector<PointControle*> tab)
{
    int k;
    Vecteur3D vec;
    tabPoints = tab;
    distanceTotal = 0;
    if (distance2Points != NULL)
    {
        delete distance2Points;
    }

    distance2Points = new float[tabPoints.size()-1];
    for (k=0;k<(int)tabPoints.size()-1;k++)
    {
        vec.x = tabPoints[k]->getCoords().x + tabPoints[k+1]->getCoords().x;
        vec.y = tabPoints[k]->getCoords().y + tabPoints[k+1]->getCoords().y;
        vec.z = tabPoints[k]->getCoords().z + tabPoints[k+1]->getCoords().z;
        distance2Points[k] = vec.getLongueur();
        distanceTotal += distance2Points[k];
    }
    updateLigne();
}

void LigneDroite::drawChemin()
{
    int k;
    Vecteur3D p1;
    Vecteur3D p2;
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glColor3f(0.6,0.6,0.2);
    //std::cout << "on va dessiner notre chemin" << std::endl;
    if (tabPoints.size()>1)
    {
        glBegin(GL_QUADS);
        for (k=0;k<tabPoints.size()-1;k++)
        {
            p1 = tabPoints[k]->getCoords();
            p2 = tabPoints[k+1]->getCoords();

                glVertex3f(p1.x-0.3 , p1.y , p1.z);
                glVertex3f(p1.x+0.3 , p1.y , p1.z);
                glVertex3f(p2.x-0.3 , p2.y , p2.z);
                glVertex3f(p2.x-0.3 , p2.y , p2.z);

                glVertex3f(p1.x , p1.y-0.3 , p1.z);
                glVertex3f(p1.x , p1.y+0.3 , p1.z);
                glVertex3f(p2.x , p2.y-0.3 , p2.z);
                glVertex3f(p2.x , p2.y-0.3 , p2.z);
        }
        glEnd();
    }
    //std::cout << "fin chemin" << std::endl;
    glPopAttrib();
}


void LigneDroite::updateLigne()
{
    int num_segment_actuel;
    bool trouve = false;
    float rapport;
    float distanceSegmentsParcourus = 0;
    int k;

    if (tabPoints.size() == 1)
    {
        vecteurActuel.x = 1;
        vecteurActuel.y = vecteurActuel.z = 0;
        posActuelle = tabPoints[0]->getCoords();
    }
    else if (vaisseauEnMarche && tabPoints.size()>1)
    {

        num_segment_actuel = 0;

        while (!trouve && num_segment_actuel < tabPoints.size()-1)
        {
            //std::cout << "segm n "<<num_segment_actuel<<std::endl;
            //std::cout << "** longueur du segment : " << distance2Points[num_segment_actuel] << "   , distanceActuelle : "
            //          << (timeCourbe)*vitesseParticule/2 << std::endl;

            if (distance2Points[num_segment_actuel] + distanceSegmentsParcourus >= (timeCourbe)*vitesseParticule/2)
            {
                trouve = true;
            }
            else
            {
                //std::cout << "segm n "<<num_segment_actuel<<std::endl;
                distanceSegmentsParcourus += distance2Points[num_segment_actuel];
                num_segment_actuel++;
            }

        }

        if (trouve)
        {
            // j'ai mon numero de segment
            rapport = ((timeCourbe)*(vitesseParticule/2) - distanceSegmentsParcourus)/ (distance2Points[num_segment_actuel]);

            //std::cout << "* DistanceParcourue : "<<distanceSegmentsParcourus<<std::endl;
            //std::cout <<"* Rapport : "<<rapport<<std::endl;


            vecteurActuel.x = tabPoints[num_segment_actuel+1]->getCoords().x - tabPoints[num_segment_actuel]->getCoords().x;
            vecteurActuel.y = tabPoints[num_segment_actuel+1]->getCoords().y - tabPoints[num_segment_actuel]->getCoords().y;
            vecteurActuel.z = tabPoints[num_segment_actuel+1]->getCoords().z - tabPoints[num_segment_actuel]->getCoords().z;


            //std::cout << "**** Vecteur actuel : "<<vecteurActuel.x << " , "<<vecteurActuel.y<<" , "<<vecteurActuel.z << std::endl;


            posActuelle.x = tabPoints[num_segment_actuel]->getCoords().x + vecteurActuel.x * rapport;
            posActuelle.y = tabPoints[num_segment_actuel]->getCoords().y + vecteurActuel.y * rapport;
            posActuelle.z = tabPoints[num_segment_actuel]->getCoords().z + vecteurActuel.z * rapport;


            std::cout << "**** Pos Actuelle : "<<posActuelle.x << " , "<<posActuelle.y<<" , "<<posActuelle.z << std::endl;
        }
    }
}


void LigneDroite::UpdateDraw()
{   
    glPushMatrix();
    updateLigne();
    drawChemin();
    /*
    std::cout << " pos actuelle : "<<posActuelle.x << " , "<<posActuelle.y<<std::endl;
    if(vecteurActuel.x != 0 && vecteurActuel.y!=0)
    {
        glPushMatrix();
        glTranslatef(posActuelle.x,posActuelle.y,posActuelle.z);
        glPushAttrib(GL_LIGHTING);
        glDisable(GL_LIGHTING);
        glColor3f(1,0.5,0.5);
        glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(vecteurActuel.x/10,vecteurActuel.y/10,0);
            glVertex3f(vecteurActuel.x/10,vecteurActuel.y/10,5);
            glVertex3f(0,0,10);
        glEnd();
        glPopAttrib();
        glPopMatrix();
    }
    */

    //std::cout << "ligne"<<std::endl;
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

void LigneDroite::UpdateName()
{
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
}



Vecteur3D LigneDroite::getVecteurActuel()
{
    return vecteurActuel;
}

Vecteur3D LigneDroite::getPosActuelle()
{
    return posActuelle;
}
