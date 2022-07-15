/****************************************************************************

 Copyright (C) 2002-2008 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.3.6.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "simpleViewer.h"

#include <qgl.h>
#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif

#define PI (3.141592653589793)

#include <cfloat>

using namespace std;

/** Initialisation des planetes du systeme **/
void Viewer::initSysteme()
{
    // Taille par rayon -> diametre/2
    Soleil.setSoleil(true);
    Soleil.setPlanete(25,0,0,0,0,0,"Soleil");
    Orion.setPlanete(10,120,16,-1,0,50,"Orion");
    SatOrion.setPlanete(3,60,0,1,0,2400,"Satellite d'Orion");
    Photon.setPlanete(6,210,8,-1,70,100,"Photon");
    Comete.setComete(true);
    Comete.setPlanete(3,350,0,-1,0,150,"Comete");

    propulsion = new NuageParticules();
    propulsion->init();

    gestionPC = new GestionPointsControle();
    ligne = new LigneDroite();
    spline = new CourbeBezier();
}

/** Creation du graphe de scène avec les paramètres en fonction
 ** du temps ***************************************************/
void Viewer::CreerGraphe()
// La plupart des rotations changeront de valeur, les translations non, sauf celle de la comete
{
    /* j'utilise des vecteurs pour changer dans updateParametersGraph() les valeurs des rotations, translation.
     * Cela permet d'aller plus vite que de parcourir l'arbre à chaque animate pour chercher la transformation */
    anglesRPhoton = new float*[4];
    anglesROrion = new float*[2];
    anglesRSatOrion = new float*[2];
    distanceComete = new float*[2]; // x et y

    distanceVaisseau = new float*[3];
    angleVaisseau = new float*[2];

    float largeurOrbites = 0.5;

    SystemeSolaire = new Node();

        SystemeSolaire.AddChildNode(gestionPC);
        gestionPC->addPoint(-300,0,0);

        SystemeSolaire.AddChildNode(spline);
        spline->init(gestionPC->getPoints());
        ligne->init(gestionPC->getPoints());

        Translation* TVaisseau = new Translation(-300,0,0,ligne,"TV");
        distanceVaisseau[0] = TVaisseau->getPtX();
        distanceVaisseau[1] = TVaisseau->getPtY();
        distanceVaisseau[2] = TVaisseau->getPtZ();
        SystemeSolaire.AddChildNode(TVaisseau);

                Rotation* RVaisseau1 = new Rotation(0,0,0,1,TVaisseau,"RV1");
                angleVaisseau[0] = RVaisseau1->getPtAngleR();
                TVaisseau->AddChildNode(RVaisseau1);

                    Rotation* RVaisseau2 = new Rotation(0,1,0,0,RVaisseau1,"RV2");
                    angleVaisseau[1] = RVaisseau2->getPtAngleR();
                    RVaisseau1->AddChildNode(RVaisseau2);

                        RVaisseau2->AddChildNode(&vaisseau);


        AxeXYZ* axes = new AxeXYZ();
        SystemeSolaire.AddChildNode(axes);

        SystemeSolaire.AddChildNode(propulsion);
        propulsion->setPosActuelle(0,0,50);

        SystemeSolaire.AddChildNode(&Soleil);

            Orbite* orbite = new Orbite(0,Photon.getDistance(),largeurOrbites,largeurOrbites);
            orbite->setRGB(1,0.4,0.4);
            Soleil.AddChildNode(orbite);
            orbite = new Orbite(0,Orion.getDistance(),largeurOrbites,largeurOrbites);
            orbite->setRGB(0.4,0.4,1);
            Soleil.AddChildNode(orbite);
            orbite = new Orbite(true,Comete.getDistance(),largeurOrbites,largeurOrbites);
            orbite->setRGB(0.5,0.5,0.5);
            Soleil.AddChildNode(orbite);

            Rotation* m_RPhoton1 = new Rotation(0,0,0,1,&Soleil,"RPhoton1");
            anglesRPhoton[0] = m_RPhoton1->getPtAngleR();
            Soleil.AddChildNode(m_RPhoton1);

                Translation* m_TPhoton = new Translation(Photon.getDistance(),0,0,m_RPhoton1,"TPhoton");
                m_RPhoton1->AddChildNode(m_TPhoton);

                    Rotation* m_RPhoton2 = new Rotation(0,0,0,1,m_TPhoton,"RPhoton2");
                    anglesRPhoton[1] = m_RPhoton2->getPtAngleR();
                    m_TPhoton->AddChildNode(m_RPhoton2);

                        // Je met directement 70° en inclinaison, cette valeur ne changera pas
                        Rotation* m_RPhoton3 = new Rotation(Photon.getInclinaison(),0,1,0,m_RPhoton2,"RPhoton3");
                        anglesRPhoton[2] = m_RPhoton3->getPtAngleR();
                        m_RPhoton2->AddChildNode(m_RPhoton3);

                            Rotation* m_RPhoton4 = new Rotation(0,0,0,1,m_TPhoton,"RPhoton4");
                            anglesRPhoton[3] = m_RPhoton4->getPtAngleR();
                            m_RPhoton3->AddChildNode(m_RPhoton4);

                                m_RPhoton4->AddChildNode(&Photon);
                                Photon.SetParentNode(m_RPhoton3);

                                    orbite = new Orbite(false,Photon.getRayon()+8,7,1);
                                    orbite->setRGB(0.4,0,0.2);
                                    orbite->setAlpha(0.8);
                                    orbite->setOnLight(false);  // lumiere pas optimisée pour eclipse
                                    Photon.AddChildNode(orbite);

            Rotation* m_ROrion1 = new Rotation(0,0,0,1,&Soleil,"ROrion1");
            anglesROrion[0] = m_ROrion1->getPtAngleR();
            Soleil.AddChildNode(m_ROrion1);

                Translation* m_TOrion = new Translation(Orion.getDistance(),0,0,m_ROrion1,"TOrion");
                m_ROrion1->AddChildNode(m_TOrion);

                    Rotation* m_ROrion2 = new Rotation(0,0,0,1,m_TOrion,"ROrion2");
                    anglesROrion[1] = m_ROrion2->getPtAngleR();
                    m_TOrion->AddChildNode(m_ROrion2);

                        m_ROrion2->AddChildNode(&Orion);
                        Orion.SetParentNode(m_ROrion2);

                    // Je dessine l'orbite du satellite d'orion
                    orbite = new Orbite(false,SatOrion.getDistance(),largeurOrbites,largeurOrbites);
                    orbite->setRGB(0.4,0.8,0.4);
                    m_TOrion->AddChildNode(orbite);

                    Rotation* m_RSatOrion1 = new Rotation(0,0,0,1,m_TOrion,"RSatOrion1");
                    anglesRSatOrion[0] = m_RSatOrion1->getPtAngleR();
                    m_TOrion->AddChildNode(m_RSatOrion1);

                        Translation* m_TSatOrion = new Translation(SatOrion.getDistance(),0,0,m_RSatOrion1,"TSatOrion");
                        m_RSatOrion1->AddChildNode(m_TSatOrion);

                            Rotation* m_RSatOrion2 = new Rotation(0,0,0,1,m_TOrion,"RSatOrion2");
                            anglesRSatOrion[1] = m_RSatOrion2->getPtAngleR();
                            m_TSatOrion->AddChildNode(m_RSatOrion2);

                                    m_RSatOrion2->AddChildNode(&SatOrion);
                                    SatOrion.SetParentNode(m_RSatOrion2);

            // Seule Translation dont la distance va changer de valeur
            Translation* m_TComete = new Translation(Comete.getDistance(),0,0,&Soleil,"TComete");
            distanceComete[0] = m_TComete->getPtX();
            distanceComete[1] = m_TComete->getPtY();
            Soleil.AddChildNode(m_TComete);

                    m_TComete->AddChildNode(&Comete);
                    Comete.SetParentNode(m_TComete);
}

/** Changement des valeurs des angles / distance comete en fonction de time **/
void Viewer::updateParametersGraph()
{
    float angle = 0;
    float angleNeg = 0;
    float distance;

    // Permet de récupérer les coordonnées x et y des planètes avec le temps
    SystemeSolaire.setTime(time);


    /** Changement pour le vaisseau **/
    if (est_spline)
    {
        *(distanceVaisseau[0]) = spline->getPosActuelle().x;
        *(distanceVaisseau[1]) = spline->getPosActuelle().y;
        *(distanceVaisseau[2]) = spline->getPosActuelle().z;

        propulsion->setPosActuelle(spline->getPosActuelle().x,spline->getPosActuelle().y,spline->getPosActuelle().z);
        propulsion->setVecDirecteurActuel(spline->getVecteurActuel().x,spline->getVecteurActuel().y,spline->getVecteurActuel().z);

        //angle = -90+vec_test.calculerAngle2DXY();
        angle = -90+spline->getVecteurActuel().calculerAngle2DXY();
        *(angleVaisseau[0]) = angle;

        //angle = vec_test.calculerAngle2DYZ();
        angle = spline->getVecteurActuel().calculerAngle2DYZ();
        *(angleVaisseau[1]) = angle;
    }
    else
    {
        *(distanceVaisseau[0]) = ligne->getPosActuelle().x;
        *(distanceVaisseau[1]) = ligne->getPosActuelle().y;
        *(distanceVaisseau[2]) = ligne->getPosActuelle().z;

        propulsion->setPosActuelle(ligne->getPosActuelle().x,ligne->getPosActuelle().y,ligne->getPosActuelle().z);
        propulsion->setVecDirecteurActuel(ligne->getVecteurActuel().x,ligne->getVecteurActuel().y,ligne->getVecteurActuel().z);

        //angle = -90+vec_test.calculerAngle2DXY();
        angle = -90+ligne->getVecteurActuel().calculerAngle2DXY();
        *(angleVaisseau[0]) = angle;

        //angle = vec_test.calculerAngle2DYZ();
        angle = ligne->getVecteurActuel().calculerAngle2DYZ();
        *(angleVaisseau[1]) = angle;
    }

    /** Changement pour photon **/
    angle = (float)Photon.getRotation()*Photon.getOrbite()*time;
    angleNeg = -angle;
    *(anglesRPhoton[0]) = angle;

        *(anglesRPhoton[1]) = angleNeg;
        // apres avoir remis photon en place, je l'incline

            angle = (float)Photon.getAnnee()*Photon.getOrbite()*time;
            *(anglesRPhoton[3]) = angle;

    /** Changement pour Orion **/
    angle = (float)Orion.getRotation()*Orion.getOrbite()*time;
    angleNeg = -angle;
    *(anglesROrion[0]) = angle;
    // ensuite on fait la translation

        angle = (float)Orion.getAnnee()*Orion.getOrbite()*time;
        *(anglesROrion[1]) = angle + angleNeg;

        /** Changement pour le satellite d'orion **/
        angle = (float)SatOrion.getRotation()*SatOrion.getOrbite()*time;
        *(anglesRSatOrion[0]) = angle + angleNeg;
        angleNeg = -angle;
        // translation pour le satellite

            *(anglesRSatOrion[1]) = angleNeg;

    /** Changement pour la comete **/
    *(distanceComete[0]) = Comete.getX();
    *(distanceComete[1]) = Comete.getY();
}

/** Gestion du temps, de la vitesse **/
void Viewer::setTimeToAdd(int t)
{
    addTime = (float)t/(float)600;
}

void Viewer::setTime()
{
    time += addTime;
    // Pour eviter de depasser la taille max des float
    if (time > FLT_MAX - 100)
    {
        std::cout<< "RAZ du temps"<<std::endl;
        time = 0;
    }

    /// Pour parcourir ma ligne
    if (VaisseauMarche)
    {
        //std::cout << "ajout temps"<<std::endl;
        spline->addtime(addTime);
        ligne->addtime(addTime);
    }
}


/** Partie pour la gestion des textures, modes d'affichage et options **/
void Viewer::setModeFilDeFer(bool etat)
{
    SystemeSolaire.setModeFilDeFer(etat);
    update();
}

void Viewer::setModeTexture(bool etat)
{
    SystemeSolaire.setModeTexture(etat);
    update();
}

void Viewer::setModeNone(bool etat)
{
    update();
}

void Viewer::setAfficherOrbite(bool etat)
{
    SystemeSolaire.setAfficherOrbite(etat);
    update();
}

void Viewer::setAxes(bool etat)
{
    SystemeSolaire.setAxes(etat);
    update();
}

void Viewer::setXYZ(bool etat)
{
    SystemeSolaire.setXYZ(etat);
    update();
}

void Viewer::setMinNearFilter(bool etat)
{
    // Probleme avec le boolean, mettre !etat permet de regler le probleme
    SystemeSolaire.setModeMinFilter(!etat);
    update();
}

void Viewer::setMagNearestFilter(bool etat)
{
    SystemeSolaire.setModeMagFilter(!etat);
    update();
}

void Viewer::setOnAnimation()
{
    startAnimation();
}

void Viewer::setOffAnimation()
{
    stopAnimation();
}

void Viewer::setLight(bool etat)
{
    if (etat)
        initLight();
    else
        offLight();
    update();
}

void Viewer::offLight()
{
    glDisable(GL_LIGHTING);
}

void Viewer::setBouclier(bool etat)
{
    if (animationIsStarted())
    {
        vaisseau.setBouclier(etat);
        update();
    }
}

void Viewer::setVaisseauMarche()
{
    if (animationIsStarted())
    {
        std::cout << "Mise en marche du vaisseau" << std::endl;
        SystemeSolaire.setVaisseauMarche(true);
        VaisseauMarche = true;
        update();
    }
}

void Viewer::setVaisseauArret()
{
    std::cout << "Arret du vaisseau" << std::endl;
    SystemeSolaire.setVaisseauMarche(false);
    VaisseauMarche = false;
}

void Viewer::setMarcheAvant_Arriere()
{
    SystemeSolaire.setMarcheArriere();
}

void Viewer::setSpline(bool etat)
{
    if (etat)
    {
        est_spline = true;
        SystemeSolaire.AddChildNode(spline);
        SystemeSolaire.RemoveChildNode(ligne);
    }
    else
    {
        est_spline = false;
        SystemeSolaire.AddChildNode(ligne);
        SystemeSolaire.RemoveChildNode(spline);
    }
}

/** Autre partie pour la gestion des effets / animate /draw ... **/

void Viewer::initLight()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    //  glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);
  const GLfloat pos[4] = {0, 0, 0, 1.0};
   const GLfloat amb[4] = {0.1, 0.1, 0.1, 1.0};
    const GLfloat dif[4] = {1.0, 1.0, 1.0, 1.0};
     const GLfloat spe[4] = {1.0, 1.0, 1.0, 1.0};
      const GLfloat spo[4] = {0, 0, 0, 1.0};
      const GLfloat con = 1.0;
       const GLfloat lin = 0.0;
        const GLfloat qua= 0.0;
      glLightfv( GL_LIGHT1, GL_AMBIENT, amb );
      glLightfv( GL_LIGHT1, GL_DIFFUSE, dif );
      glLightfv( GL_LIGHT1, GL_SPECULAR, spe );
      glLightfv( GL_LIGHT1, GL_POSITION, pos );
      glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, spo );
      glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION, con );
      glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION, lin );
      glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, qua );
}


void Viewer::draw()
{
  //std::cout << "draw" << std::endl;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0,0,0,0);
      const GLfloat pos[4] = {0, 0, 0, 1.0};    // A mettre pour le light
      glLightfv(GL_LIGHT1, GL_POSITION,pos);
  SystemeSolaire.UpdateDraw();
  drawWithNames();
/*
  glPushAttrib(GL_LIGHTING);
  glDisable(GL_LIGHTING);
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(840,0,0);
    glVertex3f(840,391,0);
    glVertex3f(0,391,0);
  glEnd();
  glPopAttrib();
*/
}

void Viewer::animate()
{
  setTime();
  updateParametersGraph();
  //testNuage();
  // std::cout << time << std::endl;
}

void Viewer::drawWithNames()
{
  glPushMatrix();
  SystemeSolaire.UpdateName();
  glPopMatrix();
}

void Viewer::postSelection(const QPoint& point )
{
  cout << "Viewer::postSelection(const QPoint& point)" << endl;

  camera()->convertClickToLine(point, orig, dir);

  bool found;

  selectedPoint = camera()->pointUnderPixel(point, found);
  selectedPoint -= 0.01f*dir;

  if (selectedName() == -1){
      cout <<"No selection" <<endl;
      cout <<"No object selected under pixel " << point.x() << ", " << point.y()<<endl;
      if (idSelected != -1)
      {
          //std::cout << "on deselectionne : "<< idSelected << std::endl;
          gestionPC->selection(idSelected-1,false);
      }
  }
  else {
      cout << "object number " << selectedName() << " selected under pixel " <<
              point.x() << ", " << point.y()<<endl;
      gestionPC->selection(selectedName()-1,true);
      if (idSelected != -1 && idSelected != selectedName())
      {
          //std::cout << "on deselectionne : "<< idSelected << std::endl;
          gestionPC->selection(idSelected-1,false);
      }
  }
  idSelected = selectedName();
}



void Viewer::mouseMoveEvent(QMouseEvent* e) {
  //cout << "Viewer::mouseMoveEvent(QMouseEvent* e)" << endl;
  if (movingObjectXY) {
    vec.x = (e->x() - pointMem.x);
    vec.y = (e->y() - pointMem.y);
    // We move the object
    //std::cout << "On le bouge de x : "<< vec.x << "  et de y : " << vec.y <<std::endl;
    gestionPC->changeControlePoint(idSelected-1,vec.x,-vec.y,0);
    pointMem.x = e->x();
    pointMem.y = e->y();
    spline->init(gestionPC->getPoints());
    ligne->init(gestionPC->getPoints());
    update();
  }
  else if (movingObjectZ) {
    vec.z = (e->y() - pointMem.z);
    //std::cout << "On le bouge de z : "<< vec.z << std::endl;
    gestionPC->changeControlePoint(idSelected-1,0,0,-vec.z);
    pointMem.z = e->y();
    spline->init(gestionPC->getPoints());
    ligne->init(gestionPC->getPoints());
    update();
  }

  // Normal QGLViewer behavior.
  QGLViewer::mouseMoveEvent(e);
}

void Viewer::mouseReleaseEvent(QMouseEvent* e) {
  cout << "Viewer::mouseReleaseEvent(QMouseEvent* e)" << endl;
  if (movingObjectXY) {
    movingObjectXY = false;
    cout << "Fin mouvement XY" << endl;
  }
  else if (movingObjectZ)
  {
      movingObjectZ = false;
      cout << "Fin mouvement Z" << endl;
  }
  QGLViewer::mouseReleaseEvent(e);
}

void Viewer::mousePressEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton)
  {
      if (idSelected != -1)
      {
          if (e->modifiers() == Qt::ShiftModifier && e->button() == Qt::LeftButton)
          {
              movingObjectXY = true;
              cout << "Begin mouvement XY" << endl;
              pointMem.x = e->x();
              pointMem.y = e->y();
          }
          else if (e->modifiers() == Qt::ControlModifier && e->button() == Qt::LeftButton)
          {
              movingObjectZ = true;
              cout << "Begin mouvement Z" << endl;
              pointMem.z = e->y();
          }
      }
      if (e->modifiers() == Qt::AltModifier)
      {
        cout << "ajout d'un point de controle" << std::endl;
        addPointControle(e);
      }
  }
  else if (e->modifiers() == Qt::ControlModifier && e->button() == Qt::RightButton
           && idSelected != -1)
  {
      std::cout << "remove PC" << idSelected << std::endl;
      removePointControle(idSelected);
  }
  update();
  QGLViewer::mousePressEvent(e);
}


void Viewer::addPointControle(QMouseEvent* e)
{
    float x = e->x()*2 - 840;
    float y = -(e->y()*2 - 391);

    gestionPC->addPoint(x,y,0);
    ligne->init(gestionPC->getPoints());
    spline->init(gestionPC->getPoints());
}


void Viewer::removePointControle(int numero)
{
    //cout << "suppresion du point "<<numero<<std::endl;
    gestionPC->removePoint(numero-1);
    spline->init(gestionPC->getPoints());
    ligne->init(gestionPC->getPoints());
    idSelected = -1;
}


void Viewer::init()
{
    est_spline = true;

  initSysteme();
  CreerGraphe();
  updateParametersGraph();
  time = 0;
  movingObjectXY = false;

  bool VaisseauMarche = false;

  SystemeSolaire.setModeMagFilter(true);
  SystemeSolaire.setModeMinFilter(true);

  test = (double*) malloc(sizeof(double)*2);

  //glEnable(GL_DEPTH_TEST);  // Test pour afficher ou non un élément en fonction de la profondeur
  initLight();  // au début, on met la lumière à True

  idSelected = -1; // rien n'est sélectionné

  // We want to restrict ourselves to a 2D viewer.
  camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
  setMouseBinding(Qt::NoModifier, Qt::LeftButton, CAMERA, SCREEN_ROTATE);
  setMouseBinding(Qt::AltModifier, Qt::LeftButton, CAMERA, NO_MOUSE_ACTION);
  setMouseBinding(Qt::ShiftModifier, Qt::LeftButton, SELECT);
  setMouseBinding(Qt::NoModifier, Qt::MouseButton(Qt::LeftButton + Qt::MidButton), CAMERA, NO_MOUSE_ACTION);
  setMouseBinding(Qt::ControlModifier, Qt::MouseButton(Qt::LeftButton + Qt::MidButton), CAMERA, NO_MOUSE_ACTION);

  // Our scene will be from -taille to taille in X and Y (the grid will be 10x10).
  setSceneRadius(300);
  showEntireScene();

  setAxisIsDrawn(false);
  setGridIsDrawn(false);

  //glDisable(GL_LIGHTING);

  //help();
}

QString Viewer::helpString() const
{
  QString text("<h2>SimpleViewer - Lab 1</h2>");
  text += "Use the mouse to move the camera around the object. ";
  text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
  text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
  text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text += "Simply press the function key again to restore it. Several keyFrames define a ";
  text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
  text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
  text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
  text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
  text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
  text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
  text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}


// A mettre dans le animate
// va créer une spirale qui part du haut du soleil
void Viewer::testNuage()
{
    float angle = 130*time*PI/180;
    //std::cout << " *** Changement de l'angle pour le test : " << (angle*180/PI) << std::endl;
    //std::cout << time*20 << std::endl;
    propulsion->setPosActuelle(0,0,50+time*20);
    propulsion->setVecDirecteurActuel(cos(angle),sin(angle),0);
    //propulsion->setVecDirecteurActuel(1,-1,1);
}
