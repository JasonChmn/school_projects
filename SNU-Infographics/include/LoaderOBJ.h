#pragma once


#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"

#include "Face.h"
#include "GroupFace.h"
#include "ChronoTime.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class LoaderOBJ
{
public:
	// Position of camera and her index, ps : index from 0 to 5 (add more if needed in this function)
	LoaderOBJ();

	void loadObj(const char* folderPath, const char* fileName, 
				std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, 
				std::vector<glm::vec3> &normals, std::vector<GroupFace> &listMaterialGroup);
	
	

private:
	void loadMtl(const char * pathFolder, const char * nameMtlFile, std::vector<GroupFace> &listMaterialGroup);
	int getIndexGroup(char* nameGroup, std::vector<GroupFace> &listMaterialGroup);

	void addFaceToList(Face &face, int index, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals, std::vector<GroupFace> &listMaterialGroup);

	glm::vec3 getIndicesFromWord(char* word);	// extract 3 indices from a text : XXX/XXX/XXX
};