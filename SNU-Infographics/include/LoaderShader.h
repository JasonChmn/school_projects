#pragma once


#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"

#include <iostream>


#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"

#include <string>
#include <iostream>
#include <fstream>

class LoadShader
{
public:
	unsigned long getFileLength(std::ifstream* file);
	int loadshader(char* filename, GLchar** ShaderSource, unsigned long* len);
	int unloadshader(GLubyte** ShaderSource);

private:
	

};

