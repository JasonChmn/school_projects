#ifndef TRANSLATION_H
#define TRANSLATION_H

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
#include <string>
#include "node.h"
#include "vecteur3d.h"

class Translation : public Node
{
public:
    Translation();
    Translation(float newX, float newY, float newZ,Node* Parent = NULL, const char* Name =NULL);
    void UpdateDraw();
    void UpdateName();

    void setTranslation(float newX, float newY, float newZ);
    void moveTranslation(float movex, float movey, float movez); // ajoute x,y,z à ceux existant

    float* getPtX();
    float* getPtY();
    float* getPtZ();

private:
    float x,y,z;
};

#endif // TRANSLATION_H
