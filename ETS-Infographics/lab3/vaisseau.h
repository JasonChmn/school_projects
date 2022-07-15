#ifndef VAISSEAU_H
#define VAISSEAU_H

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


#include "glm.h"
#include "node.h"
#include "orbite.h" // permettra de faire le bouclier

class Vaisseau : public Node
{
public:
    Vaisseau();
    void UpdateDraw();
    void UpdateName();

    // Le vaisseau est pose sur le plan xz et le haut vers les y
    void drawVaisseau();

    // Permet d'activer le bouclier qui sera sur le haut de la fusée
    void setBouclier(bool etat);

protected:
    // Permet de dessiner le bouclier (voir la direction du vaisseau sur l'axe z)
    void drawBouclier();
    GLMmodel* mon_vaisseau;
    bool bouclierOn;
    Orbite* OrbiteBouclier;
};

#endif // VAISSEAU_H
