#include "nuageparticules.h"

#define PI (3.141592653589793)

NuageParticules::NuageParticules()
{
    NULL;
}

/// L'initialisation fonctionne. Met tous les tabEstNe à false
void NuageParticules::init()
{
    float aux;
    int k;
    //std::cout << "init du nuage" << std::endl;
    nb_particules = new int;
    *nb_particules = 0;

    nb_max_particules = (int)(vitesseParticule*4);

    // Dans notre systeme, le temps va au minimum par pas de 0.001666667
    /*
     *  duree_vie / addTime = nombre de pas dans une vie
     *  Si on a nb_max_particules / nombre de pas = part_min_par_salve
     *
    */
    if (nb_max_particules > 200)
    {
        aux = 5;
    }
    else
    {
        aux = 3;
    }

    nb_particules_salve_max = (float)nb_max_particules/(particule.get_duree_vie()/0.0017)*aux;
    if (nb_particules_salve_max>6)
    {
        nb_particules_salve_max = 5;
    }
    //std::cout << "nombre de particules par salves : "<<nb_particules_salve_max<<std::endl;

    tabEstNe = new bool[nb_max_particules]();
    tabInfoParticules = new infoParticule*[nb_max_particules];

    for (k=0;k<nb_max_particules;k++)
    {
        tabEstNe[k] = false;
    }

    // On met ce cas pour la première particule, problème avec la naissanceTime sinon
    premiere_particule = true;

    // On met l'intervalle entre 2 salves pour que toutes les particules soient dessinées
    intervalle_2_salves = particule.get_duree_vie()/(float)(nb_max_particules/nb_particules_salve_max);
    //std::cout << "intervalle 2 salves : "<<intervalle_2_salves<< std::endl;

    last_time_salve = -1;   // Mis à 10 pour lancer directement la propulsion (1e boucle)
}

bool NuageParticules::nuageVide()
{
    return (*nb_particules == 0);
}


void NuageParticules::UpdateDraw()
{
    int k;
    glPushMatrix();
        //glRotatef(90,0,0,1);
        // Le updateGraphe gère tout, des données jusqu'à l'updateDraw de ses enfants
        updateGraphe();

    glPopMatrix();
}

void NuageParticules::UpdateName()
{
    NULL;
}

/** Fonction principal qui met à jour le graphe de scène, et les tableaux **/
void NuageParticules::updateGraphe()
{
    /* On regarde toutes les particules, et si elle est morte, on enleve sa partie (R,T,R) */
    int k;
    int nb_particules_reinit = 0;   // Une fois à deux, on arrête d'en ressuciter
    bool particule_actuelle_existe;

    //std::cout << " =========== Debut de la boucle For" << std::endl;

    //checkExiste();

    // Si le vaisseau est en marche, on va faire des particules
    // Si le nuage n'est pas vide, celles-ci vont disparaître ou être dessinées
    // Si c'est les deux en même temps, on crée et modifie en même temps

    if (vaisseauEnMarche || !nuageVide())
        for (k=0;k<nb_max_particules;k++)
        {
            particule_actuelle_existe = tabEstNe[k];
/*
            if (particule_actuelle_existe)
            {
                std::cout << " * " <<k << " existe" << std::endl;
            }
            else
            {
                std::cout << k <<" Non existe" << std::endl;
            }
*/
            /// Modification dans le tabEstNe, si la particule est morte, elle n'existe plus
            if (particule_actuelle_existe)
            {
                /// METTRE UN BOOL POUR LE DEBUT, PROBLEME AVEC 0
                particule.setNaissanceTime(tabInfoParticules[k]->naissanceTime);

                //std::cout << "   Elle existe. Je check pour voir si elle est morte, age : "
                //        << particule.getAge()<<std::endl;
                if (particule.est_morte())
                {
                    //std::cout << "  elle est morte, je détruis son graphe" << std::endl;
                    detruireGrapheParticule(k);
                    //std::cout << "  Suppression effectuee" << std::endl;
                    particule_actuelle_existe = false;
                }
            }
            /// Fin de la partie modification de tabEstNe et du graphe

            /** L'update des caractéristiques d'une particule existante se fait tout seul **/

            /// On va maintenant ressuciter une particule qui n'existait plus.
            /// Cela se fait si le vaisseau est en marche
            /// *** MAX 2 particules ressucitées, à changer si besoin ***
            //std::cout << "  Pour l'intervalle de temps. time - last_time_salve = "<<(time - last_time_salve)<<std::endl;
            if (!particule_actuelle_existe && nb_particules_reinit < nb_particules_salve_max
                && vaisseauEnMarche && (time - last_time_salve)>=intervalle_2_salves)
            {
                //std::cout << "***** creation du graphe pour "<<k << " fois num : "<<nb_particules_reinit<< std::endl;
                creerGrapheParticule(k);
                //std::cout << "fin de creation" << std::endl;
                nb_particules_reinit++;
                if (nb_particules_reinit == nb_particules_salve_max)
                {
                    last_time_salve = time;
                }
                if (premiere_particule)
                {
                    tabInfoParticules[k]->naissanceTime = 0;
                    premiere_particule = false;
                }
            }
            /// Fin de la création de la particule

            /// On dessine une à une toutes les particules qui existent
            if (particule_actuelle_existe)
            {
                // on a déjà mis le temps de naissance avant
                tabInfoParticules[k]->translation->UpdateDraw();
            }
            /// Fin du dessin

        }// Fin de la boucle for
    //std::cout << "Fin de la boucle for" <<std::endl;
}


void NuageParticules::creerGrapheParticule(int k)
{
    (*nb_particules)++;
    tabEstNe[k] = true;
    tabInfoParticules[k] = new infoParticule;

    tabInfoParticules[k]->naissanceTime = time;
    //std::cout << "   *** MAJ du  naissance time : "<< tabInfoParticules[k]->naissanceTime << std::endl;
    particule.setNaissanceTime(time);
    //std::cout << "   *** age de la particule maintenant : " << particule.getAge()<< std::endl;
    tabInfoParticules[k]->posNaissance = positionActuelle;
    tabInfoParticules[k]->vecDirecteur = vecDirecteurActuelle;

    /// Creation du graphe de scène
    // Je me déplace vers la position du vaisseau à l'instant où nait la particule.
    // **FIXE**
    tabInfoParticules[k]->translation = new Translation(tabInfoParticules[k]->posNaissance.x,
                                                        tabInfoParticules[k]->posNaissance.y,
                                                        tabInfoParticules[k]->posNaissance.z,
                                                        this,"t1");
    this->AddChildNode(tabInfoParticules[k]->translation);

    // on calcule l'ange de rotation1 avec le vecteur, et on oriente notre vaisseau sur XY.
    // **FIXE**
     tabInfoParticules[k]->rotation1 = new Rotation(90+tabInfoParticules[k]->vecDirecteur.calculerAngle2DXY(),0,0,1);
     tabInfoParticules[k]->translation->AddChildNode(tabInfoParticules[k]->rotation1);

     // on calcule l'ange de rotation2 avec le vecteur, et on oriente notre vaisseau sur YZ.
     // **FIXE**
      tabInfoParticules[k]->rotation2 = new Rotation(-tabInfoParticules[k]->vecDirecteur.calculerAngle2DYZ(),1,0,0);
      tabInfoParticules[k]->rotation1->AddChildNode(tabInfoParticules[k]->rotation2);


    // Maintenant, on peut placer notre particule.
    // **VARIABLE en fonction du time dans RT_particule**
    // Translation selon le nouvel axe oy
     tabInfoParticules[k]->rt = new RT_Particule();
     tabInfoParticules[k]->rotation2->AddChildNode(tabInfoParticules[k]->rt);

     tabInfoParticules[k]->rt->AddChildNode(&particule);
}

void NuageParticules::detruireGrapheParticule(int k)
{
    //std::cout << "      Debut de la suppression"<< std::endl;
    (*nb_particules)--;
    tabEstNe[k] = false;
    this->RemoveChildNode(tabInfoParticules[k]->translation);
    delete tabInfoParticules[k]->rotation1;
    delete tabInfoParticules[k]->rotation2;
    delete tabInfoParticules[k]->translation;
    delete tabInfoParticules[k]->rt;
    delete tabInfoParticules[k];
}

void NuageParticules::setPosActuelle(float x, float y, float z)
{
    positionActuelle.x = x;
    positionActuelle.y = y;
    positionActuelle.z = z;
}

void NuageParticules::setVecDirecteurActuel(float x, float y, float z)
{
    vecDirecteurActuelle.x = x;
    vecDirecteurActuelle.y = y;
    vecDirecteurActuelle.z = z;
}

void NuageParticules::setVitesse(float newVitesse)
{
    vitesseParticule = newVitesse;
}



void NuageParticules::checkExiste()
{
    std::cout << "============= VERIF ============="<< std::endl;
    int k;
    for (k=0;k<nb_max_particules;k++)
    {
        if (tabEstNe[k])
        {
            std::cout << " * " <<k << " existe"<<std::endl;
        }
        else
        {
            std::cout << k << " n'existe pas"<<std::endl;
        }
    }
    std::cout << "=============== FIN VERIF ==============="<< std::endl;
}
