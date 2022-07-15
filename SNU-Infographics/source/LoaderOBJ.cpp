#include "..\include\LoaderOBJ.h"



LoaderOBJ::LoaderOBJ()
{
}


void LoaderOBJ::loadObj(const char* folderPath, const char* fileName, 
						std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, 
						std::vector<glm::vec3> &normals, std::vector<GroupFace> &listMaterialGroup)
{

	printf("\nStart loading model, please wait ... "); ChronoTime::resetChrono();
	// Variables
	std::string nameMtlFile = "";	// To load the mtl after loop
	std::fstream file;
	std::string line;
	bool faceInLoading = false; bool moreVertices = false;	// After the "f", check if we have 3 or more vertices
	int indexMaterial = -1;	
	Face face;

	std::string strPathFile = folderPath;
	std::string strFileName = fileName;

	file.open((strPathFile+strFileName).c_str(), file.in);
	if (!file.is_open())
	{
		printf("\nCannot open %s", (strPathFile + strFileName).c_str());
		Sleep(2000);
		exit(1);
	}

	int cpt = 0; // just for printf

	//printf("\n"); ChronoTime::printTimeSince(); // TIME

	while (file.good())
	{
		file >> line;
		moreVertices = false;
		/*
		if ((cpt > 622680 && cpt < 622688)) {
			printf("\n%i - %s", cpt, line.c_str());
		}
		*/
		cpt++;
		// Start comparison of word
		if (line == "v")
		{
			glm::vec3 v;
			file >> v.x; file >> v.y; file >> v.z;
			vertices.push_back(v);
		}
		else if (line == "vt") {
			// put texture coord.
			glm::vec2 vt;
			file >> vt.x; file >> vt.y;
			textures.push_back(vt);
		}
		else if (line == "vn") {
			// put normal.
			glm::vec3 vn;
			file >> vn.x; file >> vn.y; file >> vn.z;
			normals.push_back(vn);
		}
		else if (line == "usemtl") {
			if (faceInLoading) {
				addFaceToList(face, indexMaterial, vertices, textures, normals, listMaterialGroup);
				faceInLoading = false;
			}
			// Add a new group of material to list of groups
			file >> line;
			indexMaterial = getIndexGroup((char*)line.c_str(), listMaterialGroup);
			//printf("\nMaterial %s => Group : %i ", line.c_str(), indexMaterial);
			//printf("\n"); ChronoTime::printTimeSince(); // TIME
			if (indexMaterial == -1) {
				//printf(" => This group does not exist yet, we add it to listMaterialGroup...");
				// if the group does not exist yet, we create it.
				GroupFace groupFace;
				groupFace.setNameGroup((char*)line.c_str());
				listMaterialGroup.push_back(groupFace);
				//printf("\n(%i) -> (%s)", 0, listMaterialGroup[0].nameGroup);
				indexMaterial = listMaterialGroup.size()-1;
				//printf("\nTime to create group : "); ChronoTime::printTimeSince(); // TIME
			}
		}
		else if (line == "f")
		{
			// if we had a value before, we put it in list.
			if (faceInLoading) {
				addFaceToList(face, indexMaterial, vertices, textures, normals, listMaterialGroup);
				faceInLoading = false;
			}
			glm::vec3 indices;
			for (int i = 0; i < 3; i++) {
				file >> line;
				indices = getIndicesFromWord((char*)line.c_str());
				face.indexV.push_back((int)(indices[0]));
				face.indexT.push_back((int)(indices[1]));
				//printf("\nIndice tex : %f ---> coord (%f, %f)", indices[1], textures[indices[1]][0], textures[indices[1]][1]);
				if (indices[1] == -9999) {
					face.haveTexture = false;
				} else {
					face.haveTexture = true;
				}
				face.indexN.push_back((int)(indices[2]));
				if (indices[2] == -9999) {
					face.haveNormal = false;
				} else {
					face.haveNormal = true;
				}
			}
			face.nbVertices = 3;
			faceInLoading = true;
		}
		else if (line == "s") {
			if (faceInLoading) {
				addFaceToList(face, indexMaterial, vertices, textures, normals, listMaterialGroup);
				faceInLoading = false;
			}
			// Smooth shading : ignore for the moment.
			file >> line;
		}
		else if (line == "mtllib") {
			// .mtl file for materials.
			// Loading after the loop
			file >> nameMtlFile;
			printf("\nName mtlFile ---> %s", nameMtlFile.c_str());
		}
		else if (line == "g") {
			if (faceInLoading) {
				addFaceToList(face, indexMaterial, vertices, textures, normals, listMaterialGroup);
				faceInLoading = false;
			}
			// It's for group, there is supposed to be a name after, but not in the file I use.
			// Do nothing
		}
		else if (line == "#")
		{
			/* ignoring this line */
			std::getline(file, line);
		}
		else {
			//printf("\n"); ChronoTime::printTimeSince(); // TIME
			// We have another value for f, the fourth
			face.nbVertices++;
			glm::vec3 indices = getIndicesFromWord((char*)line.c_str());
			face.indexV.push_back((int)(indices[0]));
			face.indexT.push_back((int)(indices[1]));
			if (indices[1] == -9999) {
				face.haveTexture = false;
			}
			else {
				face.haveTexture = true;
			}
			face.indexN.push_back((int)(indices[2]));
			if (indices[2] == -9999) {
				face.haveNormal = false;
			}
			else {
				face.haveNormal = true;
			}
			moreVertices = true;
			// printf(" + (%i) = {%i, %i, %i}", face.nbVertices, face.indexV[face.nbVertices - 1], face.indexT[face.nbVertices - 1], face.indexN[face.nbVertices - 1]);
			//printf("\nTime to push f : "); ChronoTime::printTimeSince(); // TIME
		}
	}
	loadMtl((strPathFile).c_str(), nameMtlFile.c_str(), listMaterialGroup);
	printf("\nEnd loading model ! Time to load the model : "); ChronoTime::printTimeSince();
}


void LoaderOBJ::loadMtl(const char * pathFolder, const char * nameMtlFile, std::vector<GroupFace> &listMaterialGroup)
{
	//printf("\n\n ==============Load MTL================ \n\n");
	std::fstream file;
	std::string strPathFolder = ""; strPathFolder += pathFolder;
	std::string strNameMtlFile = ""; strNameMtlFile += nameMtlFile;
	std::string strPathTotal = strPathFolder + strNameMtlFile;
	file.open(strPathTotal, file.in);
	if (!file.is_open())
	{
		printf("\nCannot open %s", (strPathTotal).c_str());
		Sleep(5000);
		exit(1);
	}
	std::string line;
	int cpt = 0;

	// Parameters of face
	int Ns = 5;
	glm::vec3 Ka, Kd, Ks;
	// others
	int indexGroup;

	// Loop
	while (file.good())
	{
		file >> line;
		//printf("\n%i %s", cpt++, line.c_str());
		if (line == "newmtl") {
			// name of group
			file >> line;
			indexGroup = getIndexGroup((char*)line.c_str(), listMaterialGroup);
		}
		else if (line == "Ns") {
			// Ns
			file >> Ns;
		}
		else if (line == "Ka") {
			// Ka
			for (int i = 0; i < 3; i++) {
				file >> Ka[i];
			}
		}
		else if (line == "Kd") {
			// Kd
			for (int i = 0; i < 3; i++) {
				file >> Kd[i];
			}
		}
		else if (line == "Ks") {
			// Ks
			for (int i = 0; i < 3; i++) {
				file >> Ks[i];
			}
		}
		else if (line == "map_Kd") {
			// name of texture
			file >> line;
			std::string nameTextureAux = "";
			nameTextureAux += line;
			// Add group to list
			if (indexGroup != -1) {
				//printf("\nKS : %f, %f, %f", Ks.x, Ks.y, Ks.z);
				listMaterialGroup[indexGroup].setParameters(Ns, Ka, Kd, Ks, (char*)nameTextureAux.c_str(), (char*)(strPathFolder.c_str()));
			}
		} 
		else if (line == "illum" || line == "Ni") {
			// Ignore for the moment
			file >> line;
		}
		else if (line == "Tf") {
			// Ignore for the moment
			file >> line >> line >> line;
		}
		else if (line == "bump") {
			// Ignore for the moment
			std::getline(file, line);
		}
	}
}




int LoaderOBJ::getIndexGroup(char * nameGroup, std::vector<GroupFace> &listMaterialGroup)
{
	int index = -1;
	int i = 0;
	//printf("\nGetIndex, sizelist : %i", listMaterialGroup.size());
	while (i < listMaterialGroup.size() && index == -1) {
		if (listMaterialGroup[i].compareNameGroup(nameGroup)) {
			index = i;
		}
		i++;
	}
	return index;
}

glm::vec3 LoaderOBJ::getIndicesFromWord(char * word)
{
	glm::vec3 indices(0.0f,0.0f,0.0f);
	int cptStart = 0;
	int cptVec = 0;
	int lengthWord = strlen(word);
	int value = 0;
	std::string valueString = "";
	for (int i = 0; i <= lengthWord; i++) {
		if (word[i] == '/' || i == lengthWord) {
			if (valueString != "") {
				std::istringstream(valueString) >> value;
				indices[cptVec] = (float)(value - 1);
			}
			else {
				value = -9999;
			}
			cptVec++;
			valueString = "";
		}
		else {
			valueString += word[i];
		}
	}
	if (cptVec == 1) {
		indices[1] = -9999;
		indices[2] = -9999;
	}
	else if (cptVec == 2) {
		indices[2] = -9999;
	}
	return indices;
}



void LoaderOBJ::addFaceToList(Face &face, int index, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals, std::vector<GroupFace> &listMaterialGroup) {
	//printf("\nNumber faces : %i", face.nbVertices);
	//printf("\n ADDED");
	//printf("\n"); ChronoTime::printTimeSince(); // TIME
	listMaterialGroup[index].addFace(face, vertices, textures, normals);
	// clear face
	face.clearFace();
	//printf("\nTime to add FACE : "); ChronoTime::printTimeSince(); // TIME
}