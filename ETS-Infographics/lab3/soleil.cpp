#include "soleil.h"
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

void Soleil::drawWithName()
{
    GLUquadricObj* Sphere;
    Sphere = gluNewQuadric();
    gluSphere(Sphere,taille,20,20);
    gluDeleteQuadric(Sphere);
}

void Soleil::draw()
{
    //std::cout << "je dessine une étoile de rayon et distance : "<< taille << " , " << distance << std::endl;

    glColor3f(1,1,1);
    GLfloat white[] = {1.0, 1.0, 1.0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);

    if (modeTexture)
    {
        //std::cout<<"on va mettre les textures"<<std::endl;
        glEnable(GL_TEXTURE_2D);
    }

    glPushMatrix();
    GLUquadricObj* Sphere;
    Sphere = gluNewQuadric();

    if (modeTexture)
    {
        loadTexture();
        gluQuadricTexture(Sphere,1);
    }
    else if (modeFilDeFer)
    {
        gluQuadricDrawStyle(Sphere,GLU_LINE);	// Si on veut l'afficher en mode fil de fer
    }

    gluSphere(Sphere,taille,20,20);
    gluDeleteQuadric(Sphere);
    glPopMatrix();

    if (modeTexture)
    {
        glDisable(GL_TEXTURE_2D);
        delete image;
    }

    glFlush();
}

