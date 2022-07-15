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

#include <QGLViewer/qglviewer.h>

#include "node.h"
#include "planete.h"
#include "translation.h"
#include "rotation.h"
#include "orbite.h"
#include "vaisseau.h"
#include "nuageparticules.h"
#include "axexyz.h"
#include "lignedroite.h"
#include "courbebezier.h"

#include "gestionpointscontrole.h"
#include "vecteur3d.h"

#include <cmath>
#include <QMouseEvent>

#include "test.h"

class Viewer : public QGLViewer
{
    Q_OBJECT
protected :  

  Vaisseau* test_v;

  virtual void draw();
  virtual void animate();
  virtual void init();
  virtual QString helpString() const;

  virtual void drawWithNames();
  virtual void postSelection(const QPoint& point);
  virtual void mouseMoveEvent(QMouseEvent* e);
  virtual void mouseReleaseEvent(QMouseEvent* e);
  virtual void mousePressEvent(QMouseEvent* e);

  virtual void setTime();

  virtual void initSysteme();
  virtual void CreerGraphe();

  virtual void updateParametersGraph();

  // test pour le nuage de particule
  virtual void testNuage();

public slots:
  virtual void setTimeToAdd(int t);

  virtual void setModeFilDeFer(bool etat);
  virtual void setModeTexture(bool etat);
  virtual void setModeNone(bool etat);

  virtual void setLight(bool etat);

  virtual void setAfficherOrbite(bool etat);

  virtual void setAxes(bool etat);
  virtual void setXYZ(bool etat);

  virtual void setMinNearFilter(bool etat);
  virtual void setMagNearestFilter(bool etat);

  virtual void setOnAnimation();
  virtual void setOffAnimation();

  virtual void setBouclier(bool etat);
  virtual void setVaisseauMarche();
  virtual void setVaisseauArret();
  virtual void setMarcheAvant_Arriere();

  virtual void setSpline(bool etat);

private:

  void addPointControle(QMouseEvent *e);
  void removePointControle(int numero);

  void setMode();
  void initLight();
  void offLight();

  bool movingObjectXY;
  bool movingObjectZ;
  float addTime;
  float time;

  qglviewer::Vec orig, dir, selectedPoint;

  double* test;

  int idSelected;

  Node SystemeSolaire;

  Planete Soleil;
  Planete Orion;
  float** anglesROrion;
  Planete SatOrion;
  float** anglesRSatOrion;
  Planete Photon;
  float** anglesRPhoton;
  Planete Comete;
  float** distanceComete;

  Vaisseau vaisseau;
  float** distanceVaisseau; // x,y,z
  float** angleVaisseau;

  NuageParticules* propulsion;

  GestionPointsControle* gestionPC;

  Vecteur3D pointMem;
  Vecteur3D vec;

  Vecteur3D vec_test;


  bool VaisseauMarche;

  LigneDroite* ligne;
  CourbeBezier* spline;

  bool est_spline;
};
