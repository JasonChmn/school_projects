#include "orbite.h"
#define PI (3.141592653589793)

Orbite::Orbite(bool est_comete, int NewRayon, float newLargeurAnneau, float newHauteurAnneau)
{
    comete = est_comete;
    rayon = NewRayon;
    largeurAnneau = newLargeurAnneau;
    hauteurAnneau = newHauteurAnneau;
    alpha = 0;
    onLight = false;
}

void Orbite::UpdateDraw()
{
    glPushMatrix();

    //Je dessine mon orbite
    if (modeAfficherOrbite)
    {
        drawOrbite();
    }
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

void Orbite::setRayon(float newRayon)
{
    rayon = newRayon;
}

void Orbite::setRGB(float newr, float newg, float newb)
{
    r = newr;
    g = newg;
    b = newb;
}

void Orbite::setAlpha(float newAlpha)
{
    alpha = newAlpha;
}

void Orbite::setOnLight(bool etat)
{
    onLight = etat;
}

void Orbite::setLargeurAnneau(float newLargeurAnneau)
{
    largeurAnneau = newLargeurAnneau;
}

void Orbite::setHauteurAnneau(float newHauteurAnneau)
{
    hauteurAnneau = newHauteurAnneau;
}

void Orbite::UpdateName()
{
    NULL;
}

void Orbite::drawOrbite()
{
    glPushMatrix();
    if (!onLight)
    {
        glPushAttrib(GL_LIGHTING);
        glDisable(GL_LIGHTING);
    }
        float angle;
        double angleRad;
        float distance;
        float distanceMin;
        /// un point tous les 6 degrés si rayon supérieur à 10
        /// Sinon tous les 20 degrés
        int marge = 6;
        if (rayon <10)
            marge = 20;
        double cosinus,sinus;

        glColor4f(r,g,b,alpha);
        glBegin(GL_QUAD_STRIP);
            if (comete)
            {
                for (angle=0;angle<=360;angle=angle+marge)
                {
                    angleRad = (double)angle * PI / 180.0;
                    cosinus = cos(angleRad);
                    sinus = sin(angleRad);

                    distance = rayon;
                    distanceMin = rayon/2;

                    glVertex3f((distance-(float)largeurAnneau)*cosinus,(distanceMin-(float)largeurAnneau)*sinus,0);
                    glVertex3f(distance*cosinus,distanceMin*sinus,0);
                }
            }
            else
            {
                for (angle=0; angle<=360; angle = angle+marge)
                {
                    angleRad = (double)angle * PI / 180.0;
                    cosinus = cos(angleRad);
                    sinus = sin(angleRad);
                    distance = rayon;
                    glVertex3f((distance-(float)largeurAnneau)*cosinus,(distance-(float)largeurAnneau)*sinus,0);
                    glVertex3f(distance*cosinus,distance*sinus,0);
                }
            }
        glEnd();

        // Je fais une sortie de croix pour pouvoir la voir de côté aussi
        glBegin(GL_QUAD_STRIP);
            if (comete)
            {
                for (angle=0;angle<=360;angle=angle+marge)
                {
                    angleRad = (double)angle * PI / 180.0;
                    cosinus = cos(angleRad);
                    sinus = sin(angleRad);

                    distance = rayon;
                    distanceMin = rayon/2;

                    glVertex3f((distance-(float)largeurAnneau/2)*cosinus,
                               (distanceMin-(float)largeurAnneau/2)*sinus,
                               -(float)largeurAnneau/2);
                    glVertex3f((distance-(float)largeurAnneau/2)*cosinus,
                               (distanceMin-(float)largeurAnneau/2)*sinus,
                               (float)largeurAnneau/2);
                }
            }
            else
            {
                for (angle=0; angle<=360; angle = angle+marge)
                {
                    angleRad = (double)angle * PI / 180.0;
                    cosinus = cos(angleRad);
                    sinus = sin(angleRad);
                    distance = rayon;
                    glVertex3f((distance-(float)largeurAnneau/2)*cosinus,
                               (distance-(float)largeurAnneau/2)*sinus,
                               -(float)hauteurAnneau/2);
                    glVertex3f((distance-(float)largeurAnneau/2)*cosinus,
                               (distance-(float)largeurAnneau/2)*sinus,
                               (float)hauteurAnneau/2);
                }
            }
        glEnd();
    if (!onLight)
    {
        glPopAttrib();
    }
    glPopMatrix();
}
