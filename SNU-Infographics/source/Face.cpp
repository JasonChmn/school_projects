#include "Face.h"

Face::Face( )
{
	this->nbVertices = 0;
	this->haveTexture = false;
	this->haveNormal = false;
}

void Face::clearFace()
{
	this->nbVertices = 0;
	indexV.clear();
	indexT.clear();
	indexN.clear();
}
