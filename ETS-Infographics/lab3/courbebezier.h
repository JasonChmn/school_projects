#ifndef COURBEBEZIER_H
#define COURBEBEZIER_H

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif


#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "pointcontrole.h"
#include "vecteur3d.h"

class CourbeBezier : public Node
{
public:
    CourbeBezier();

    void addtime(float newTime);

    void draw();
    void UpdateDraw();

    void init(std::vector<PointControle *> points);

    void UpdateCoord();

    Vecteur3D getVecteurActuel();
    Vecteur3D getPosActuelle();

protected:

    void updateDistanceTotale();
    void cleanCourbe();

    float timeCourbe;

    float distanceTotale;

    std::vector<PointControle *> courbe;

    std::vector<PointControle *> courbeClean;



    Vecteur3D posActuelle;
    Vecteur3D vecteurActuel;
};

#endif // COURBEBEZIER_H
