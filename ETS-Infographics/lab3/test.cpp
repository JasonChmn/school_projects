#include "test.h"

TEST::TEST()
{

}

void TEST::UpdateDraw()
{
    glPushAttrib(GL_LIGHTING);
        GLUquadricObj* Sphere;
        Sphere = gluNewQuadric();
        gluSphere(Sphere,20,20,20);
    glPopAttrib();
}
