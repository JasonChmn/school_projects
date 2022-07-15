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
#include "ui_untitled.h"
#include <qapplication.h>
#include <qframe.h>
#include <QBoxLayout>
#include <math.h>
#include <cstdlib>  // pour rand
#include <time.h>
#include <QGroupBox>

#define GLUT_DISABLE_ATEXIT_HACK
#ifdef Q_OS_WIN32
#include "GL/glut.h"
#endif
#ifdef Q_OS_MAC
#include "GLUT/glut.h"
#endif
#include <QFile>
#include <QDir>
#include <string>


int main(int argc, char** argv)
{
  // Pour l'aléatoire
    srand(time(NULL));

  QString CurrentDir = QDir::currentPath();
  std::cout<<CurrentDir.toStdString()<<std::endl;

  // Read command lines arguments.
  QApplication application(argc,argv);

  glutInit(&argc, argv);

  QMainWindow* mainWindow = new QMainWindow;
  Ui_MainWindow form1;
  form1.setupUi(mainWindow);

  // Instantiate and layout the viewer.
  Viewer *v = new Viewer();
  QLayout *layout = new QHBoxLayout;
  layout->addWidget(v);
  form1.frame->setLayout(layout);

  /* Speed */
  form1.horizontalSlider->setMinimum(0);
  form1.horizontalSlider->setMaximum(50);
  QObject::connect(form1.horizontalSlider,SIGNAL(valueChanged(int)),v,SLOT(setTimeToAdd(int)));

  /* Mode d'affichage : texture... */
  form1.actionFil_de_fer->setCheckable(true);
  form1.actionTexture->setCheckable(true);
  form1.actionTexture->setChecked(true);
  form1.actionNone_2->setCheckable(true);

  /* Eclairage */
  form1.actionEclairage->setCheckable(true);
  form1.actionEclairage->setChecked(true);
  QObject::connect(form1.actionEclairage,SIGNAL(toggled(bool)),v,SLOT(setLight(bool)));

  /* Orbite */
  form1.actionOrbite->setCheckable(true);
  form1.actionOrbite->setChecked(true);
  QObject::connect(form1.actionOrbite,SIGNAL(toggled(bool)),v,SLOT(setAfficherOrbite(bool)));

  /* Filter */
  form1.actionNearest->setCheckable(true);

  form1.actionNearest_2->setCheckable(true);

  form1.actionLinear->setCheckable(true);
  form1.actionLinear_mipmap_linear->setCheckable(true);

  /* Axes des planetes */
  form1.actionAxes->setCheckable(true);
  form1.actionAxes->setChecked(true);
  QObject::connect(form1.actionAxes,SIGNAL(toggled(bool)),v,SLOT(setAxes(bool)));

  /* Axes XYZ */
  form1.actionAxe_XYZ->setCheckable(true);
  form1.actionAxe_XYZ->setChecked(false);
  QObject::connect(form1.actionAxe_XYZ,SIGNAL(toggled(bool)),v,SLOT(setXYZ(bool)));

  /* Marche arrier et avant */
  QObject::connect(form1.pushButton_5,SIGNAL(clicked()),v,SLOT(setMarcheAvant_Arriere()));

  /** Partie pour le menu exclusif **/
  /* Pour les textures */
  QActionGroup *grp = new QActionGroup( mainWindow );
  grp->addAction(form1.actionFil_de_fer);
  grp->addAction(form1.actionTexture);
  grp->addAction(form1.actionNone_2);
  grp->setExclusive(true);
  QObject::connect(form1.actionFil_de_fer,SIGNAL(toggled(bool)),v,SLOT(setModeFilDeFer(bool)));
  QObject::connect(form1.actionTexture,SIGNAL(toggled(bool)),v,SLOT(setModeTexture(bool)));
  QObject::connect(form1.actionNone_2,SIGNAL(toggled(bool)),v,SLOT(setModeNone(bool)));

  /* Mag */
  QActionGroup *grpA = new QActionGroup( mainWindow );
  grpA->addAction(form1.actionLinear);
  grpA->addAction(form1.actionNearest_2);
  grpA->setExclusive(true);
  form1.actionNearest_2->setChecked(true);
  QObject::connect(form1.actionLinear,SIGNAL(toggled(bool)),v,SLOT(setMagNearestFilter(bool)));

  /* Min */
  QActionGroup *grpB = new QActionGroup( mainWindow );
  grpB->addAction(form1.actionLinear_mipmap_linear);
  grpB->addAction(form1.actionNearest);
  grpB->setExclusive(true);
  form1.actionNearest->setChecked(true);
  QObject::connect(form1.actionLinear_mipmap_linear,SIGNAL(toggled(bool)),v,SLOT(setMinNearFilter(bool)));

  /* mode spline ou ligne */
  QActionGroup *grpC = new QActionGroup( mainWindow );
  grpC->addAction(form1.actionSpline);
  grpC->addAction(form1.actionDroite);
  grpC->setExclusive(true);
  form1.actionSpline->setCheckable(true);
  form1.actionDroite->setCheckable(true);
  form1.actionSpline->setChecked(true);
  QObject::connect(form1.actionSpline,SIGNAL(toggled(bool)),v,SLOT(setSpline(bool)));

  /* Bouclier du vaisseau */
  form1.actionBouclier->setCheckable(true);
  form1.actionBouclier->setChecked(false);
  QObject::connect(form1.actionBouclier,SIGNAL(toggled(bool)),v,SLOT(setBouclier(bool)));

  /* Marche et arrêt du vaisseau */
  QObject::connect(form1.pushButton_3,SIGNAL(clicked()),v,SLOT(setVaisseauMarche()));
  QObject::connect(form1.pushButton_4,SIGNAL(clicked()),v,SLOT(setVaisseauArret()));

  /** Partie pour l'animation **/
  /* Pour les boutons start & stop de l'animation */
  QObject::connect(form1.pushButton,SIGNAL(clicked()),v,SLOT(setOnAnimation()));
  QObject::connect(form1.pushButton_2,SIGNAL(clicked()),v,SLOT(setOffAnimation()));


  mainWindow->show();

  // Run main loop.
  return application.exec();
}
