#ifndef PARTICULE_H
#define PARTICULE_H

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif

#include "node.h"
#include "Vecteur3D.h"

/* Pour le placement de la particule, on fera juste translation -> rotation -> translation
 * Choix => facilité pour la gestion des coordonnées :
 *          Translation pour remettre le point là où il est né
 *          On fait une rotation en fonction du vecteur directeur
 *          Translation pour faire bouger la particule en fonction du temps
*/
class Particule : public Node
{
public:
    // Je vais partager la même instance de particule dans nuageParticules
    // Au maximum, on aura 50 particules
    Particule();
    ~Particule();

    void UpdateDraw();
    void UpdateName();

    // Permet de mettre le temps où est nait la particule
    void setNaissanceTime(float newNaissanceTime);

    bool est_morte();

    float getAge();

    float naissanceTime;

    float get_duree_vie();

private:
    // Va calculer le bleu en fonction du temps
    void calculerRGB();

    // Sera appelé dans draw pour la gestion de l'age de la particule
    void setAge();

    // Fonction qui fait appel à calculerRGB et fait un setAge avant
    void drawParticule();

    // On calculera le bleu avec l'age de la particule
    // si l'age de la particule dépasse 0.10 (en unité de temps de notre système), elle n'est plus affichée
    float ageParticule;

    float taille;   // grosseur des particules

    float red;
    float green;
    float blue;

    float duree_vie;
};

#endif // PARTICULE_H
