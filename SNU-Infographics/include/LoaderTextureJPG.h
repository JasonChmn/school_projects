
#ifndef LoaderTextureJPG_H
#define LoaderTextureJPG_H

#include <iostream>

#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"
#include "SOIL.h"

class LoaderTextureJPG
{
public:
	static int createNewTexture(const char* pathTextureJPG, GLint paramWrap, GLint paramFilter);
};

#endif