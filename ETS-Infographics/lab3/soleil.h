#ifndef SOLEIL_H
#define SOLEIL_H
#include "node.h"
#include "planete.h"

class Soleil : public Node, public Planete
{
public:
    Soleil();
private:
    void drawWithName();
    void draw();
};

#endif // SOLEIL_H
