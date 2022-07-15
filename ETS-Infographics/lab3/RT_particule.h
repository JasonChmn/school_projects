#ifndef RT_PARTICULE_H
#define RT_PARTICULE_H

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif
#include <iostream>
#include <cmath>
#include <math.h>
#include "node.h"

#include "Vecteur3D.h"

#include <cstdlib>  // pour rand

class RT_Particule : public Node
{
public:
    /// On rentre le naissanceTime pour pouvoir calculer sa translation
    /// La vitesse quand à elle est par rapport au vaisseau, c'est une valeur à voir
    RT_Particule();

    void UpdateDraw();
    void UpdateName();

    RT_Particule* getPtRt();

    void setNaissanceTime();

private:
    void transformation();

    float aleatoireSigne();

    float naissanceTime;
    float angle_1_a_10; // angle obtenu aleatoirement dont la valeur est entre -10 et 10 degrés
    float angle_aleatoire;  // Permet de placer la particule aléatoirement sur le cone

    float age;  // comme l'age de la particule
};

#endif // RT_PARTICULE_H
