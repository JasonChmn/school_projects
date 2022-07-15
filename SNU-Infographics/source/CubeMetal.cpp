#include "CubeMetal.h"

CubeMetal::CubeMetal(const char* pathToTexture, float sizeSide, glm::vec3 positionCenter, bool onlyFace1, int numberDivisionSide)
{
	this->onlyFace1 = onlyFace1;
	this->nbDivisionsSide = numberDivisionSide;

	this->sizeSide = sizeSide;
	
	this->posCenterCube[0] = positionCenter.x;
	this->posCenterCube[1] = positionCenter.y;
	this->posCenterCube[2] = positionCenter.z;
	this->posCenterCube[3] = 1.0f;

	this->indexVertexFace[0] = { 0,1,2,3 };
	this->indexVertexFace[1] = { 1,5,6,2 };
	this->indexVertexFace[2] = { 5,4,7,6 };
	this->indexVertexFace[3] = { 0,3,7,4 };
	this->indexVertexFace[4] = { 2,6,7,3 };
	this->indexVertexFace[5] = { 0,1,5,4 };

	this->normalFace[0] = { 0,0,1 };
	this->normalFace[1] = { 1,0,0 };
	this->normalFace[2] = { 0,0,-1 };
	this->normalFace[3] = { -1,0,0 };
	this->normalFace[4] = { 0,1,0 };
	this->normalFace[5] = { 0,-1,0 };

	float demiSide = sizeSide / 2.0f;
	this->posVertexOrigin[0] = { -demiSide,-demiSide,demiSide };
	this->posVertexOrigin[1] = { demiSide,-demiSide,demiSide };
	this->posVertexOrigin[2] = { demiSide,demiSide,demiSide };
	this->posVertexOrigin[3] = { -demiSide,demiSide,demiSide };
	this->posVertexOrigin[4] = { -demiSide,-demiSide,-demiSide };
	this->posVertexOrigin[5] = { demiSide,-demiSide,-demiSide };
	this->posVertexOrigin[6] = { demiSide,demiSide,-demiSide };
	this->posVertexOrigin[7] = { -demiSide,demiSide,-demiSide };

	if (onlyFace1) {
		printf("\nCreation list one face");
		loadTexture(pathToTexture);
		createListOneFace();
	}
}

void CubeMetal::drawCube()
{
	if (!this->onlyFace1) {
		setMaterial();
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			int indexVertex;
			for (int nFace = 0; nFace < 6; nFace++) {
				for (int nVertex = 0; nVertex < 4; nVertex++) {
					indexVertex = this->indexVertexFace[nFace][nVertex];
					glNormal3f( this->normalFace[nFace].x, this->normalFace[nFace].y, this->normalFace[nFace].z);
					glVertex3f(	this->posCenterCube[0] + this->posVertexOrigin[indexVertex][0],
								this->posCenterCube[1] + this->posVertexOrigin[indexVertex][1], 
								this->posCenterCube[2] + this->posVertexOrigin[indexVertex][2] );
				}
			}
		glEnd();
	}
	else {
		setMaterial();
		glCallList(this->IDlist);
	}
}

float CubeMetal::getRadiusCube()
{
	return sizeSide/2.0f;
}

float * CubeMetal::getPtrCenterCube()
{
	return this->posCenterCube;
}

void CubeMetal::changeShininess(float value)
{
	this->Ns[0] = value;
}


void CubeMetal::loadTexture(const char * pathToTexture)
{
	this->IDtexture = LoaderTextureJPG::createNewTexture(pathToTexture, GL_REPEAT, GL_LINEAR);
}

void CubeMetal::createListOneFace()
{
	float halfSize = this->sizeSide / 2.0f;
	this->IDlist = glGenLists(1);

	glNewList(this->IDlist, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, this->IDtexture);
		float sizeBloc = calculateSizeBloc(this->nbDivisionsSide, sizeSide);
		// draw front
		glBegin(GL_QUADS);
			glm::vec2 coordAux;
			float z = this->posCenterCube[2];
			for (int i = 0; i < nbDivisionsSide*nbDivisionsSide; i++) {
				glNormal3f(0.0f, 0.0f, 1.0f);
				//printf("\nValue i : %i -> coord : %f, %f", i, coordAux.x, coordAux.y);
				coordAux = getCoordBloc(i, nbDivisionsSide, sizeBloc);
				coordAux.x -= sizeSide / 2.0f;
				coordAux.y -= sizeSide / 2.0f;
				// v4
				if (IDtexture != -1) {
					glTexCoord2f(0.0f, 1.0f);
				}
				glVertex3f(coordAux.x, coordAux.y + sizeBloc, z);
				// v3
				if (IDtexture != -1) {
					glTexCoord2f(1.0f, 1.0f);
				}
				glVertex3f(coordAux.x + sizeBloc, coordAux.y + sizeBloc, z);
				// v2
				if (IDtexture != -1) {
					glTexCoord2f(1.0f, 0.0f);
				}
				glVertex3f(coordAux.x + sizeBloc, coordAux.y, z);
				// v1
				if (IDtexture != -1) {
					glTexCoord2f(0.0f, 0.0f);
				}
				glVertex3f(coordAux.x, coordAux.y, z);
			}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();
}

float CubeMetal::calculateSizeBloc(int numberBlocsOneLIne, float sizeSide)
{
	return sizeSide / (float)(numberBlocsOneLIne);
}

glm::vec2 CubeMetal::getCoordBloc(int indexBloc, int numberBlocsOneLIne, float sizeBloc)
{
	glm::vec2 coord;
	if (indexBloc < numberBlocsOneLIne*numberBlocsOneLIne) {
		//printf(" *calcul coord index(%i) and nbBlocsLine(%i), sizeBloc : %f* ", indexBloc, numberBlocsOneLIne, sizeBloc);
		coord[0] = (float)(indexBloc%numberBlocsOneLIne) * sizeBloc;	// bottomLeft x of bloc
		coord[1] = floor((float)(indexBloc / numberBlocsOneLIne)) * sizeBloc;
	}
	return coord;
}

void CubeMetal::setMaterial()
{
	//printf("\nK values : %f, rgb(%f,%f,%f), %f, %i", Ka[0], Kd[0], Kd[1], Kd[2], Ks[0], this->Ns);
	glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);
	glMaterialfv(GL_FRONT, GL_SHININESS, Ns);
}