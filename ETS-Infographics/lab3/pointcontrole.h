#ifndef POINTCONTROLE_H
#define POINTCONTROLE_H

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif

#include "node.h"
#include "vecteur3d.h"

class PointControle : public Node
{
public:
    PointControle();
    PointControle(float x,float y,float z);
    void UpdateDraw(void);
    void UpdateName(void);

    void setCoords(float x, float y, float z);
    void moveCoords(float x, float y, float z);
    Vecteur3D getCoords();

    void setSelectionne(bool etat);

private:
    void drawPC();

    float hauteur;
    float largeur;

    Vecteur3D coords;

    bool selectionne;
};

#endif // POINTCONTROLE_H
