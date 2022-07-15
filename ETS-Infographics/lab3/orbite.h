#ifndef ORBITE_H
#define ORBITE_H

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif
#include <iostream>
#include <math.h>

#include "node.h"

class Orbite : public Node
{
public:
    Orbite(bool est_comete, int NewRayon, float newLargeurAnneau, float newHauteurAnneau);

    void UpdateDraw();
    void UpdateName();

    void drawOrbite();

    void setRGB(float newr, float newg, float newb);
    void setAlpha(float newAlpha);

    void setOnLight(bool etat);

    void setRayon(float newRayon);

    void setLargeurAnneau(float newLargeurAnneau);
    void setHauteurAnneau(float newHauteurAnneau);

protected:
    float rayon;
    float largeurAnneau;
    float hauteurAnneau;
    bool comete;
    bool onLight;

    float r,g,b;
    float alpha;
};

#endif // ORBITE_H
