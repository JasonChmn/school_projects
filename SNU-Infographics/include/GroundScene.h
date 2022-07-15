#pragma once

#include <iostream>

#include "LoaderTextureJPG.h"

#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"


class GroundScene
{
public:
	GroundScene(const char* pathToTexture, float size, int numberDivisionLine);
	void drawGround();

private:
	int IDlist;

	float Ka[3] = {1, 1 , 1 };
	float Kd[3] = { 1, 1 , 1 };
	float Ks[3] = { 0.0f, 0.0f , 0.0f};
	GLint Ns = 5;

	void loadTexture(const char* pathToTexture);
	GLuint IDtexture;
	void setMaterial();

	void createList(float sizeGround, int numberDivisionLine);

	float calculateSizeBloc(int numberBlocsOneLIne, float sizeGround);
	glm::vec2 getCoordBloc(int indexBloc, int numberBlocsOneLIne, float sizeBloc);	// (x,y)
};
