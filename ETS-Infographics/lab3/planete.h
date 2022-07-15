#ifndef PLANETE_H
#define PLANETE_H

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
#include <QImage>
#include <string>
#include "node.h"

class Planete : public Node
{
    public:

        void UpdateDraw();
        void UpdateName();

        /* rotation +1 ou -1 pour horaire ou antihoraire */
        Planete(Node* Parent = NULL, const char* Name =NULL);
        void setPlanete(int newTaille, int newDistance, int newAnnee,
                        int newRotation, int newInclinaison, int newOrbite,
                        QString newNomPlanete);
        void drawPlanete();
        void drawWithName();

        int getDistance();
        int getRotation();
        int getOrbite();
        int getInclinaison();
        int getAnnee();
        int getRayon();

        float getX();
        float getY();

        void setSoleil(bool etat);
        void setComete(bool etat);

    protected:
        void appliquerTexture();
        void loadTexture();

        void afficherAxe();

        bool est_soleil;
        bool est_comete;

        int taille; // rayon
        int distance;   // centre à centre
        int annee;  // jours
        int rotation;   // +1 si horaire, -1 sinon par rapport à Z+
        int inclinaison;    // plan XZ par rapport à Z+
        int orbite; // vitesse angulaire en degré
        QString nomPlanete;

        QString path;
        QImage *  image;
        QImage tex;
        GLuint textureId;

        GLfloat rgb;
        GLfloat alpha;

};

#endif // PLANETE_H
