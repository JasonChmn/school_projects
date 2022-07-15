#pragma once

#include "Face.h"

#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"

#include "LoaderTextureJPG.h"
#include "ChronoTime.h"

#include <iostream>
#include <string>
#include <vector>

class GroupFace
{
public:
	// Position of camera and her index, ps : index from 0 to 5 (add more if needed in this function).
	GroupFace( );
	void setParameters(int Ns, glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, char* nameTexture, char* pathFolder);
	void setNameGroup(char* nameGroup);
	void addFace(Face faceToAdd, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals);

	bool compareNameGroup(char* nameGroup);	// return -1 if we do not find the name, 0 to X if we find it.

	// Initialization to do before drawing
	void createListToDraw(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals);
	//
	void draw();
	void sayHello();

	void drawSquareTexturedTest();

	// Variables
	int Ns; // specular exponent
	glm::vec3 Ka, Kd, Ks; // ambiant, diffuse, specular
	char* nameTexture; 
	char* nameGroup;
	char* pathFolder;
	std::vector<Face> listFaces;
	int numberTotalFaces;
	int IDListRoot;
	GLuint IDTexture;

private :
	bool textureItinitialized;
	std::vector<GLuint> IDlists;	// sublists
	bool rootListInitialized;

	void setMaterial();
	void createTexture();

	void drawGroup(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals);
	// Create a list that will be add to IDLists, with a fixed number of faces. Then clear the vector of faces.
	void createDrawListFromListFaces(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals);
};