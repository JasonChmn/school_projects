#ifndef ROTATION_H
#define ROTATION_H

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif
#include "planete.h"
#include <iostream>
#include <math.h>
#include <string>
#include "node.h"
#include "Vecteur3D.h"

class Rotation : public Node
{
    public:
        Rotation();
        Rotation(float newAngle, int new_ex, int new_ey, int new_ez,
                 Node* Parent = NULL, const char* Name =NULL);
        Rotation(Node* node);

        virtual void UpdateDraw();
        virtual void UpdateName();

        void setAngle(float newAngle);
        void setAxeRotation(int new_ex, int new_ey, int new_ez);

        float* getPtAngleR();

    private:
        float angle;
        int ex;
        int ey;
        int ez;
};
#endif // ROTATION_H
