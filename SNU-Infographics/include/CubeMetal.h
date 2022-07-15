#pragma once

#include <iostream>

#include "LoaderTextureJPG.h"

#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"


class CubeMetal
{
public:
	CubeMetal(const char* pathToTexture, float sizeSide, glm::vec3 positionCenter, bool onlyFace1, int numberDivisionSide);
	void drawCube();

	float getRadiusCube();
	float* getPtrCenterCube();

	void changeShininess(float value);

private:
	int IDlist;

	bool onlyFace1;
	int nbDivisionsSide;

	float sizeSide;
	float posCenterCube[4];
	glm::vec4 indexVertexFace[6];
	glm::vec3 normalFace[6];
	glm::vec3 posVertexOrigin[8];

	float Ka[3] = { 1, 1 , 1 };
	float Kd[3] = { 0.3f , 0.3f , 0.3f };
	//float Ks[3] = { 0.4f, 0.4f , 0.4f };
	float Ks[3] = {1,1,1};
	float Ns[1] = { 200 };

	void loadTexture(const char* pathToTexture);
	GLuint IDtexture;

	void createListOneFace();
	float calculateSizeBloc(int numberBlocsOneLIne, float sizeSide);
	glm::vec2 getCoordBloc(int indexBloc, int numberBlocsOneLIne, float sizeBloc);	// (x,y)
	void CubeMetal::setMaterial();
};