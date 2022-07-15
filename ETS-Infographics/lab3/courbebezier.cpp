#include "courbebezier.h"


CourbeBezier::CourbeBezier()
{
    NULL;
}

void  debugDraw(float points[4][3])
{

    glPointSize(10);
    glBegin(GL_POINTS);

glColor3f(1,1,0);
    glVertex3f(points[0][0], points[0][1], points[0][2]);
     glColor3f(0,1,0);
    glVertex3f(points[1][0], points[1][1], points[1][2]);
    glVertex3f(points[2][0], points[2][1], points[2][2]);
     glColor3f(1,1,0);
    glVertex3f(points[3][0], points[3][1], points[3][2]);

    glEnd( );
}


float* GetPoint(float points[4][3],int i) {
    // return 1st point
    if (i<0) {
        return	points[0];
    }
    // return last point
    if (i<4)
        return points[i];

    return points[3];
}

void CourbeBezier::UpdateDraw()
{
    glPushMatrix();
    draw();
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

void CourbeBezier::init(std::vector<PointControle *> points)
{
    float Points[4][3];
    int virage=25;
    Vecteur3D path;
    Vecteur3D next;
    Vecteur3D bsx;
    unsigned int LOD;
    LOD = 100;

if (points.size() != 0)
{
    posActuelle = points[0]->getCoords();
    vecteurActuel.x = 1;
    vecteurActuel.y = 0;
    vecteurActuel.z = 0;
}


courbe.clear();
//courbe.resize(points.size()*LOD);

// use the parametric time value 0 to 1
if(points.size() >1)
{
    for(size_t k = 0; k < points.size() - 1; ++k)
    {

      Points[0][0]=points[k]->getCoords().x;
      Points[0][1]=points[k]->getCoords().y;
      Points[0][2]=points[k]->getCoords().z;
      Points[3][0]=points[k+1]->getCoords().x;
      Points[3][1]=points[k+1]->getCoords().y;
      Points[3][2]=points[k+1]->getCoords().z;

      if(k!=0)//on recupere la bissextrice precedente
      {
          Points[1][0]=Points[0][0]+bsx.x*virage;
          Points[1][1]=Points[0][1]+bsx.y*virage;
          Points[1][2]=Points[0][2]+bsx.z*virage;
      }else{//premier point

          Points[1][0]=Points[0][0];
          Points[1][1]=Points[0][1];
          Points[1][2]=Points[0][2];
      }

      path = points[k]->getCoords().dif(points[k+1]->getCoords());




       if( k < points.size() - 2)// tant qu il nous reste une courbe a calculer
       {
           next = points[k+1]->getCoords().dif(points[k+2]->getCoords());
           next.normalize();
           virage=path.normalize()/3;
           bsx = path.sum(next);
           Points[2][0]=Points[3][0]-bsx.x*virage;
           Points[2][1]=Points[3][1]-bsx.y*virage;
           Points[2][2]=Points[3][2]-bsx.z*virage;
       }else{//derniere courbe

           Points[2][0]=Points[3][0];
           Points[2][1]=Points[3][1];
           Points[2][2]=Points[3][2];
       }




  for(int start_cv=-3,j=0;j!=7;++j,++start_cv)
    for(int i=0;i!=LOD;++i) {


        float t = (float)i/(LOD-1);

        // the t value inverted
        float it = 1.0f-t;

        // calculate blending functions
        float b0 = it*it*it/6.0f;
        float b1 = (3*t*t*t - 6*t*t +4)/6.0f;
        float b2 = (-3*t*t*t +3*t*t + 3*t + 1)/6.0f;
        float b3 =  t*t*t/6.0f;

        // sum the control points mulitplied by their respective blending functions
        float x = b0 * GetPoint(Points, start_cv + 0 )[0] +
                  b1 * GetPoint(Points, start_cv + 1 )[0] +
                  b2 * GetPoint(Points, start_cv + 2 )[0] +
                  b3 * GetPoint(Points, start_cv + 3 )[0] ;

        float y = b0 * GetPoint(Points, start_cv + 0 )[1] +
                  b1 * GetPoint(Points, start_cv + 1 )[1] +
                  b2 * GetPoint(Points, start_cv + 2 )[1] +
                  b3 * GetPoint(Points, start_cv + 3 )[1] ;

        float z = b0 * GetPoint(Points, start_cv + 0 )[2] +
                  b1 * GetPoint(Points, start_cv + 1 )[2] +
                  b2 * GetPoint(Points, start_cv + 2 )[2] +
                  b3 * GetPoint(Points, start_cv + 3 )[2] ;



        PointControle* buff= new PointControle(x,y,z);
        courbe.push_back(buff);


       // glVertex3f(x,y,z);

        }

    }
}
    courbeClean.clear();
    cleanCourbe();
    updateDistanceTotale();
}


void CourbeBezier::cleanCourbe()
{
    int k;
    Vecteur3D mem;
    mem.x =0;
    mem.y =0;
    mem.z =0;
    for (k=0;k<courbe.size();k++)
    {
        if (courbe[k]->getCoords().egaleEnviron(mem))
        {
            NULL;
        }
        else
        {
            courbeClean.push_back(courbe[k]);
            mem = courbe[k]->getCoords();
        }
    }
    //std::cout << "ancienne taille : "<<courbe.size()<< "  ----- nouvelle taille : "<<courbeClean.size()<<std::endl;
}


void CourbeBezier::addtime(float newTime)
{
    //std::cout << "timeCourbe "<<timeCourbe << std::endl;
    UpdateCoord();
    if (!marcheArriere)
    {
        timeCourbe += newTime;
        if (timeCourbe*vitesseParticule/2 > distanceTotale)
        {
            std::cout << "RAZ pos" << std::endl;
            timeCourbe = 0;
        }
    }
    else
    {
        timeCourbe -= newTime;
        if (timeCourbe*vitesseParticule/2 < 0)
        {
            std::cout << "RAZ neg" << std::endl;
            timeCourbe = distanceTotale * 2 / vitesseParticule;
        }
    }
}


void CourbeBezier::updateDistanceTotale()
{
    int k;
    distanceTotale = 0;
    Vecteur3D aux;
    if (courbeClean.size()>1)
    {
        for (k=0;k<courbeClean.size()-1;k++)
        {
            aux = (courbeClean[k]->getCoords()).dif(courbeClean[k+1]->getCoords());
            //std::cout << "aux : " <<aux.x << "  , et coord x courbe : "<< courbeClean[k]->getCoords().x<<std::endl;
            distanceTotale += aux.getLongueur();
        }
    }
    std::cout << "distance totale : "<<distanceTotale<<std::endl;
}



void CourbeBezier::UpdateCoord()
{
    int k;
    int pos = (int)((timeCourbe*vitesseParticule/2)/distanceTotale * (float)courbeClean.size());
    posActuelle=courbeClean[pos]->getCoords();
    vecteurActuel=courbeClean[pos]->getCoords().dif(courbeClean[pos+1]->getCoords());
}

void CourbeBezier::draw()
{
    glColor4f(1,0,0,0);
    glPointSize(1);
    glLineWidth(1);

    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);

    glBegin( GL_LINE_STRIP );




      for(size_t i = 0; i < courbe.size() ; ++i)
      {

          Vecteur3D buff = courbe[i]->getCoords();
          glVertex3f(buff.x,buff.y,buff.z);
      }


     glEnd();

}


Vecteur3D CourbeBezier::getVecteurActuel()
{
    return vecteurActuel;
}

Vecteur3D CourbeBezier::getPosActuelle()
{
    return posActuelle;
}
