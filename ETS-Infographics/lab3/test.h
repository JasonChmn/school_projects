#ifndef TEST_H
#define TEST_H

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

#include "node.h"

class TEST : public Node
{
public:
    TEST();
    void UpdateDraw();
};

#endif // TEST_H
