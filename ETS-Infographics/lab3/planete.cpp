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
#include "node.h"
#include <string.h>

#define PI (3.141592653589793)

using namespace std;

Planete::Planete(Node *Parent, const char *Name)
{
    taille = 0;
    distance = 0;
    annee = 0;
    rotation = 0;
    inclinaison = 0;

    rgb = 0.0;
    alpha = 0.1;

    est_soleil = false;

    m_Parent = Parent;
    m_Name = Name;
}

void Planete::setSoleil(bool etat)
{
    est_soleil = etat;
}

void Planete::setComete(bool etat)
{
    est_comete = etat;
}

float Planete::getX()
{
    float angle = (float)rotation*orbite*time;
    float angleRad = (double)angle * PI / 180.0;
    float cosinus = cos(angleRad);

    float x = distance*cosinus;
    return x;
}

float Planete::getY()
{
    float angle = (float)rotation*orbite*time;
    float y;
    if (est_comete)
    {
        float angleRad = (double)angle * PI / 180.0;
        float sinus = sin(angleRad);
        float distanceMin = (float)distance/2;
        y = distanceMin*sinus;
    }
    else
        y = (float)distance*sin(angle*2*M_PI/360);
    return y;
}

void Planete::loadTexture() {
    image = new QImage();

     if(image->load( path ))
     {
        //std::cout << "on a chargé l'image"<<std::endl;
        tex = QGLWidget::convertToGLFormat(*image);
        glGenTextures(1, &textureId); //Make room for our texture
        //gluQuadricDrawStyle(Sphere,GLU_LINE);	// Si on veut l'afficher en mode fil de fer
        glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
        //Map the image to the texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
        //Bottom
        if (minNearest)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }

        if (magNearest)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
     }
}

int Planete::getRayon()
{
    return taille;
}

int Planete::getDistance()
{
    return distance;
}

int Planete::getRotation()
{
    return rotation;
}

int Planete::getOrbite()
{
    return orbite;
}

int Planete::getInclinaison()
{
    return inclinaison;
}

int Planete::getAnnee()
{
    return annee;
}

void Planete::setPlanete(int newTaille, int newDistance, int newAnnee,
                         int newRotation, int newInclinaison, int newOrbite,
                         QString newNomPlanete)
{
    taille = newTaille;
    distance = newDistance;
    annee = newAnnee;
    rotation = newRotation;
    inclinaison = newInclinaison;
    orbite = newOrbite;
    nomPlanete = newNomPlanete;
    path = nomPlanete +".jpg";
    if (est_soleil)
        rgb = alpha = 1;
    loadTexture();
}


/************************************************************************/
/************ MON UPDATE ************************************************/
void Planete::UpdateDraw()
{
    glPushMatrix();

    //Je dessine ma planète
    drawPlanete();

    //J'update le reste
    if(!m_children.empty())
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(m_children[i] != NULL)
            {
                m_children[i]->UpdateDraw();
            }
        }
    }
    glPopMatrix();
}

void Planete::UpdateName()
{
    glPushMatrix();

    //Je dessine ma planète
    drawWithName();

    //J'update le reste
    if(!m_children.empty())
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(m_children[i] != NULL)
            {
                m_children[i]->UpdateName();
            }
        }
    }
    glPopMatrix();
}

/***********************************************************************/
/***********************************************************************/
void Planete::drawWithName()
{
    /// Il faudrait donner un numero au planète etc...

    NULL;

}

/***********************************************************************/

void Planete::afficherAxe()
{
    //std::cout << "on affiche l'axe" << std::endl;
    glPushAttrib(GL_LIGHTING);
        glDisable(GL_LIGHTING);
    int demi_longueur = taille+5;
    glColor3f(0.5,1,0.2);
    glBegin(GL_QUADS);
        // Je dessine une croix
        glVertex3f(-0.2,0,-demi_longueur);
        glVertex3f(-0.2,0,demi_longueur);
        glVertex3f(0.2,0,demi_longueur);
        glVertex3f(0.2,0,-demi_longueur);

        glVertex3f(0,-0.2,-demi_longueur);
        glVertex3f(0,-0.2,demi_longueur);
        glVertex3f(0,0.2,demi_longueur);
        glVertex3f(0,0.2,-demi_longueur);

        // Je fait la base de la pyramide pour la flèche
        glVertex3f(0.5,-0.5,demi_longueur);
        glVertex3f(0.5,0.5,demi_longueur);
        glVertex3f(-0.5,0.5,demi_longueur);
        glVertex3f(-0.5,-0.5,demi_longueur);
    glEnd();

    glColor3f(0,1,1);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,0,demi_longueur+5);
        glVertex3f(0.5,-0.5,demi_longueur);
        glVertex3f(0.5,0.5,demi_longueur);
        glVertex3f(-0.5,0.5,demi_longueur);
        glVertex3f(-0.5,-0.5,demi_longueur);
    glEnd();
    glPopAttrib();
}

/***********************************************************************/

void Planete::appliquerTexture()
{
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    if (minNearest)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    if (magNearest)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/***********************************************************************/

void Planete::drawPlanete()
{
    //std::cout << "je dessine une planète de rayon et distance : "<< taille << " , " << distance << std::endl;



    glPushMatrix();


    glColor3f(1,1,1);
    GLfloat white[] = {rgb, rgb, rgb, alpha};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);

    if (modeTexture)
    {
        //std::cout<<"on va mettre les textures"<<std::endl;
        glEnable(GL_TEXTURE_2D);
    }

    /* Je crée ma Quadric */
    GLUquadricObj* Sphere;
    Sphere = gluNewQuadric();

    /* Pour les textures 1/2 */
    if (modeTexture)
    {
        appliquerTexture();
        gluQuadricTexture(Sphere,1);
    }
    /* Si on veut l'afficher en mode fil de fer */
    else if (modeFilDeFer)
    {
        gluQuadricDrawStyle(Sphere,GLU_LINE);
    }
    gluSphere(Sphere,taille,20,20);
    gluDeleteQuadric(Sphere);

    glPopMatrix();

    /* Pour les textures 2/2 */
    if (modeTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }

    if (modeAxes)
        afficherAxe();

    //glFlush ();
}
