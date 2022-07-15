#ifndef LIGNEDROITE_H
#define LIGNEDROITE_H

#include "node.h"
#include "vecteur3d.h"
#include "pointcontrole.h"

/// retournera une position et un vecteur en fonction de time
class LigneDroite : public Node
{
public:
    LigneDroite();

    void UpdateDraw(void);
    void UpdateName(void);


    void init(std::vector<PointControle *> tab);

    void addtime(float newTime);

    Vecteur3D getVecteurActuel();
    Vecteur3D getPosActuelle();

private:
    void drawChemin();
    void updateLigne();

    std::vector<PointControle*> tabPoints;

    float* distance2Points;

    float distanceTotal;

    float distanceActuel;

    float timeCourbe;

    Vecteur3D posActuelle;
    Vecteur3D vecteurActuel;
};

#endif // LIGNEDROITE_H
