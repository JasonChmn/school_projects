#ifndef VECTEUR3D_H
#define VECTEUR3D_H

#include <cstddef>
#include <stdio.h>
#include <cstring>
#include <iostream>

#include <math.h>

class Vecteur3D
{
public:
    float x,y,z;

    Vecteur3D();

    // En degré
    /* Permet de calculer l'angle d'un vecteur 2D sur le plan XY par rapport à ox */
    // Arrangee pour que cela fonctionne avec mn systeme de particule
    float calculerAngle2DXY();
    float vraiAngleXY();    // radian
    /* Permet de calculer l'angle d'un vecteur 2D sur le plan YZ par rapport à oy */
    // Arrangee pour que cela fonctionne avec mn systeme de particule
    float calculerAngle2DYZ();
    float vraiAngleYZ();    // radian
    /* Permet de calculer l'angle d'un vecteur 2D sur le plan XZ par rapport à ox */
    float calculerAngle2DXZ();
    float vraiAngleXZ(); // radian

    float normalize();
    Vecteur3D dif(Vecteur3D vec);

    Vecteur3D sum(Vecteur3D vec);

    float getLongueur();

    bool egaleEnviron(Vecteur3D autre);
};

#endif // VECTEUR3D_H
