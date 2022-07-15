#include "..\include\GroundScene.h"



GroundScene::GroundScene(const char * pathToTexture, float size, int numberDivisionLine)
{
	loadTexture(pathToTexture);
	createList(size, numberDivisionLine);
}

void GroundScene::drawGround()
{
	//printf("\nK values : %f, rgb(%f,%f,%f), %f, %i", Ka[0], Kd[0], Kd[1], Kd[2], Ks[0], this->Ns);
	glCallList(this->IDlist);
}


void GroundScene::loadTexture(const char* pathToTexture)
{
	this->IDtexture = LoaderTextureJPG::createNewTexture(pathToTexture, GL_REPEAT, GL_LINEAR);
}

void GroundScene::setMaterial()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, (float*)this->Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (float*)this->Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, (float*)this->Ks);
	glMaterialiv(GL_FRONT, GL_SHININESS, (const GLint*)&(this->Ns));
}

void GroundScene::createList(float sizeGround, int numberDivisionLine)
{
	float halfSize = sizeGround / 2;
	this->IDlist = glGenLists(1);

	glNewList(this->IDlist, GL_COMPILE);
		setMaterial();
			glBindTexture(GL_TEXTURE_2D, this->IDtexture);
				float sizeBloc = calculateSizeBloc(numberDivisionLine, sizeGround);
				// draw front
				glDisable(GL_CULL_FACE);
				glBegin(GL_QUADS);
					glm::vec2 coordAux;
					for (int i = 0; i < numberDivisionLine*numberDivisionLine; i++) {
						glNormal3f(0.0f, 1.0f, 0.0f);
						//printf("\nValue i : %i -> coord : %f, %f", i, coordAux.x, coordAux.y);
						coordAux = getCoordBloc(i, numberDivisionLine, sizeBloc);
						coordAux.x -= sizeGround / 2.0f, 0.0f;
						coordAux.y -= sizeGround / 2.0f;
						// v4
						if (IDtexture != -1) {
							glTexCoord2f(0.0f, 1.0f);
						}
						glVertex3f(coordAux.x, 0.0f, coordAux.y + sizeBloc);
						// v3
						if (IDtexture != -1) {
							glTexCoord2f(1.0f, 1.0f);
						}
						glVertex3f(coordAux.x + sizeBloc, 0.0f, coordAux.y + sizeBloc);
						// v2
						if (IDtexture != -1) {
							glTexCoord2f(1.0f, 0.0f);
						}
						glVertex3f(coordAux.x + sizeBloc, 0.0f, coordAux.y);
						// v1
						if (IDtexture != -1) {
							glTexCoord2f(0.0f, 0.0f);
						}
						glVertex3f(coordAux.x, 0.0f, coordAux.y);
					}	
				glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		glEnd();
	glEndList();
}

float GroundScene::calculateSizeBloc(int numberBlocsOneLIne, float sizeGround)
{
	return sizeGround/(float)(numberBlocsOneLIne);
}

glm::vec2 GroundScene::getCoordBloc(int indexBloc, int numberBlocsOneLIne, float sizeBloc)
{
	glm::vec2 coord;
	if (indexBloc < numberBlocsOneLIne*numberBlocsOneLIne) {
		//printf(" *calcul coord index(%i) and nbBlocsLine(%i), sizeBloc : %f* ", indexBloc, numberBlocsOneLIne, sizeBloc);
		coord[0] = (float)(indexBloc%numberBlocsOneLIne) * sizeBloc;	// bottomLeft x of bloc
		coord[1] = floor((float)(indexBloc / numberBlocsOneLIne)) * sizeBloc ;
	}
	return coord;
}
