#include "particule.h"

Particule::Particule()
{
    ageParticule = 0;
    naissanceTime = -1;

    red = 0;
    green = 0;
    blue = 0;

    duree_vie = 0.3 + ((float)rand()/(float)RAND_MAX);

    taille = 0.5;
}

Particule::~Particule()
{

}

void Particule::UpdateDraw()
{
    glPushMatrix();
        drawParticule();
    glPopMatrix();
}

void Particule::UpdateName()
{
    NULL;
}



/** Autres fonctions de ma classe particules **/
void Particule::setNaissanceTime(float newNaissanceTime)
{
    naissanceTime = newNaissanceTime;
    setAge();
}

void Particule::calculerRGB()
{
    float aux = ageParticule/(duree_vie - duree_vie/6);
    red = 0.24 + aux*0.76; // Red : jusqu'à 1
    green = 0.20 + aux*0.80; // Blue : jusqu'à 1
    blue = 0.92 + aux*0.08; // Blue : jusqu'à 1
}

void Particule::setAge()
{
    ageParticule = time - naissanceTime;
}

bool Particule::est_morte()
{
    bool morte = false;
    // Si elle est trop vieille, ou qu'elle n'est pas encore née
    if (ageParticule > duree_vie)
    {
        morte = true;
    }
    return morte;
}

void Particule::drawParticule()
{
    //std::cout << "ça dessine une particule" << std::endl;
    calculerRGB();

    glPushAttrib(GL_LIGHTING);
        glDisable(GL_LIGHTING);
        glColor4f(red,green,blue,0.5);
        glBegin(GL_QUADS);
            glVertex3f(-taille,-taille,0);
            glVertex3f(-taille,taille,0);
            glVertex3f(taille,taille,0);
            glVertex3f(taille,-taille,0);

            glVertex3f(0,-taille,-taille);
            glVertex3f(0,-taille,taille);
            glVertex3f(0,taille,taille);
            glVertex3f(0,taille,-taille);
        glEnd();
    glPopAttrib();

}

float Particule::getAge()
{
    return ageParticule;
}

float Particule::get_duree_vie()
{
    return duree_vie;
}
