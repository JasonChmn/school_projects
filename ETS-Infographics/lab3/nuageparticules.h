#ifndef NUAGEPARTICULES_H
#define NUAGEPARTICULES_H

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif

#include "particule.h"
#include "Vecteur3D.h"
#include "rotation.h"
#include "translation.h"
#include "RT_particule.h"


/************************************//**
 Quand on est au vecteurDirecteur (0,0,0)
 Le nuage se fait vers les -y
**//************************************/
/*** Je vais gérer les particules au niveau de cette classe,  *******
 *** et pas au niveau de simpleViewer (choix pour la compréhension) */
/// Trois valeurs possibles à changer : vitesseParticule dans Node pour la vitesse (et donc la longueur de la trainee)
///                                         -> Le nb_max_particule se fait dans NuageParticules
///                                     duree_vie dans Particule pour le temps qu'elle va rester
///                                     taille dans Particule pour la grosseur des blocs de fumée
class NuageParticules : public Node
{
public:
    NuageParticules();

    void init();

    void UpdateDraw();
    void UpdateName();

    /* Dans cette fonction, on regarde le tabNaissanceTimeParticules, et si la particule
     * est morte (méthode est_morte() de particule), on met la particule à time dans le tableau
     * et on change son vecteur avec le nouveau du vaisseau (voir sur la courbe de bézier */
    // On l'appellera dans le updateDraw
    /* On met aussi à jour le graphe de scène */
    void updateGraphe();

    void setPosActuelle(float x, float y, float z);

    // Le vecteur directeur est celui vers ou doivent se diriger les particules
    void setVecDirecteurActuel(float x, float y, float z);

    void setVitesse(float newVitesse);

private:

    void creerGrapheParticule(int k);

    void detruireGrapheParticule(int k);

    bool nuageVide();

    Node* aux;

    int nb_particules_salve_max;

    int* nb_particules;

    /* On partage la même instance de particule     *
     * Au maximum, on aura 50 particules            */
    Particule particule;

    Vecteur3D vecDirecteurActuelle;  // va être changé en permanence, pour
    Vecteur3D positionActuelle;

    struct infoParticule
    {   
        // On garde ici la naissanceTime de chacune des particules
        float naissanceTime;

        // On garde les vecteurs de chacune des particules
        Vecteur3D vecDirecteur;
        Vecteur3D posNaissance;

        // Je garde les pointeurs pour les supprimer par la suite ou les modifier
        Rotation* rotation1;
        Rotation* rotation2;
        Translation* translation;
        RT_Particule* rt;
    };

    bool* tabEstNe;  // Dit si la particule existe, si oui elle aura de la mémoire allouée

    int nb_max_particules;
    infoParticule** tabInfoParticules;


    // test
    void checkExiste();

    bool premiere_particule;    // Il y a un problème avec la première particule, donc on enlève ce cas

    float intervalle_2_salves;
    float last_time_salve;
};

#endif // NUAGEPARTICULES_H
