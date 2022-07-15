#ifndef AXEXYZ_H
#define AXEXYZ_H

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif
#include <iostream>

#include "node.h"

class AxeXYZ: public Node
{
public:
    AxeXYZ();
    void UpdateDraw(void);
    void UpdateName(void);
private:
    void drawAxe();
    float hauteur;
    float largeur;
    float largeur2;
};

#endif // AXEXYZ_H
