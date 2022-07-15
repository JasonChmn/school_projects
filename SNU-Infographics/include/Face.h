#pragma once

#include <vector>

// PS : here we can have from 3 to 4 vertices only
class Face
{
public:
	// Position of camera and her index, ps : index from 0 to 5 (add more if needed in this function)
	Face( );
	void clearFace();

	// Variables
	int nbVertices;
	std::vector<int> indexV;
	std::vector<int> indexT;
	bool haveTexture;
	std::vector<int> indexN;
	bool haveNormal;
};