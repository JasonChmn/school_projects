#include "RT_particule.h"

RT_Particule::RT_Particule()
{
    float aux;
    // génération aléatoire de l'angle entre -10 et 10
    angle_1_a_10 = -10 + (float)rand()/(float)RAND_MAX * 20;

    // génération aléatoire de l'angle entre 0 et 360
    if (angle_1_a_10 <= 0.0)
    {
        aux = -angle_1_a_10;
    }
    else
    {
        aux = angle_1_a_10;
    }
    angle_aleatoire = aleatoireSigne()*(float)rand()/(float)RAND_MAX * (20 - 2*aux);

    naissanceTime = time;
}

float RT_Particule::aleatoireSigne()
{
    int aux = rand();
    if (aux < RAND_MAX/2)
    {
        return (float)-1;
    }
    else
    {
        return (float)1;
    }
}

void RT_Particule::UpdateDraw()
{
    glPushMatrix();
    transformation();

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

void RT_Particule::UpdateName()
{
    NULL;
}

void RT_Particule::transformation()
{
    // Les particules doivent aller dans l'axe Y de la fusée à l'instant où la particule est née
    glRotatef(angle_1_a_10,0,0,1);  // Sur l'axe Z, pour réaliser un cone en 2d sur xy
    glRotatef(angle_aleatoire,1,0,0);   // pour réaliser un cone en 3d
    glTranslatef(0,(time - naissanceTime)*vitesseParticule,0);
}


RT_Particule* RT_Particule::getPtRt()
{
    return this;
}

void RT_Particule::setNaissanceTime()
{
    naissanceTime = time;
}
