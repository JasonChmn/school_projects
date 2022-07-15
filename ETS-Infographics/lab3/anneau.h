#ifndef ANNEAU_H
#define ANNEAU_H

#include "node.h"

class Anneau : public Node
{
public:
    Anneau(float newRayon);

    void setRayon(float newRayon);

private:
    float rayon;
};

#endif // ANNEAU_H
