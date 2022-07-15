#ifndef COURBE_H
#define COURBE_H
#include "vecteur3d.h"
#include "node.h"
#include "pointcontrole.h"

class Courbe : public Node
{
public:
    Courbe();
    virtual void init(std::vector<PointControle *> points);
    void UpdateDraw(void);
    void UpdateName(void);

    Vecteur3D getVecteurActuel();
    Vecteur3D getPosActuelle();

protected:

    Vecteur3D posActuelle;
    Vecteur3D vecteurActuel;
};

#endif // COURBE_H
