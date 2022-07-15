#include "vecteur3d.h"

#define PI (3.141592653589793)

Vecteur3D::Vecteur3D()
{
    x = y = z = 0;
}

// Fonctionne au dixieme de degré près
float Vecteur3D::calculerAngle2DXY()
{
    float angle = atan2(y,x)*180/PI;
    if (angle < 0)
    {
        angle = 360+angle;
    }

    if (y>0 && x<0)
    {
        NULL;
    }
    else if (y<0)
    {
        angle += 180;
    }
    else if (x<0)
    {
        angle += 180;
    }

    //std::cout << " dans XY , angle : "<<angle<<std::endl;
    return angle;
}

float Vecteur3D::calculerAngle2DYZ()
{
    float angle = atan2(z,y)*180/PI;
    if (angle < 0)
    {
        angle = 360+angle;
    }

    if (y == 0)
    {
        angle = calculerAngle2DXZ();
    }

    //std::cout << " - dans YZ , angle : "<<angle<<std::endl;
    return angle;
}

float Vecteur3D::calculerAngle2DXZ()
{
    float angle = atan2(z,x)*180/PI;
    if (angle < 0)
    {
        angle = 360+angle;
    }
    //std::cout << " --- dans XZ , angle : "<<angle<<std::endl;
    return angle;
}

float Vecteur3D::vraiAngleXY()
{
    return atan2(y,x);
}

float Vecteur3D::vraiAngleYZ()
{
    return atan2(z,y);
}

float Vecteur3D::vraiAngleXZ()
{
    return atan2(z,x);
}

Vecteur3D Vecteur3D::sum(Vecteur3D vec)
{
    Vecteur3D result;
    result.x=(vec.x+this->x)/2;
    result.y=(vec.y+this->y)/2;
    result.z=(vec.z+this->z)/2;

    return result;
}


float Vecteur3D::getLongueur()
{
    return sqrt(x*x+y*y+z*z);
}

float Vecteur3D::normalize()
{
    float norm=sqrtf(x*x+y*y+z*z);

    x=x/norm;
    y=y/norm;
    z=z/norm;
    return norm;
}

Vecteur3D Vecteur3D::dif(Vecteur3D vec)
{
    Vecteur3D result;
    result.x=(vec.x-this->x);
    result.y=(vec.y-this->y);
    result.z=(vec.z-this->z);

    return result;

}

bool Vecteur3D::egaleEnviron(Vecteur3D autre)
{
    float marge = 0.1;
    bool vrai =false;
    if ((autre.x - x <marge) && (autre.x - x >-marge) &&
        (autre.y - y <marge) && (autre.y - y >-marge) &&
        (autre.z - z <marge) && (autre.z - z >-marge) )
    {
        vrai = true;
    }
    return vrai;
}
