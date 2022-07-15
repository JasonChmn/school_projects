#ifndef GESTIONPOINTSCONTROLE_H
#define GESTIONPOINTSCONTROLE_H

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
#include "pointcontrole.h"
#include "translation.h"

#include <vector>

/// Dans ma gestion des points, quand on les ajoute, ils sont au début dans
/// la liste d'enfant et dans tabPtPoints
class GestionPointsControle : public Node
{
public:
    GestionPointsControle();
    void UpdateDraw(void);
    void UpdateName(void);

    int getNbPoints();
    std::vector<PointControle *> getPoints(); // Obtient le point dans tabPtPoints

    void addPoint(float x, float y, float z); // fait appel à addChild() et met le pointeur dans le tabPtPoints

    void removePoint(int numero);   // supprime le point à la numero place. De 0 à max

    void changeControlePoint(int numero, float x, float y, float z);

    void selection(int numero, bool etat);

    Vecteur3D getCoord(int numero);

private:
    std::vector<PointControle*> tabPoints;  // je garde un pointeur vers les child pour les modifier
    std::vector<Translation*> tabTranslation;
};

#endif // GESTIONPOINTSCONTROLE_H
