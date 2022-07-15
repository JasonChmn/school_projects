#include "GroupFace.h"

GroupFace::GroupFace()
{
	this->Ns = 5;
	this->Ka = glm::vec3(0, 0, 0);
	this->Kd = glm::vec3(0, 0, 0);
	this->Ks = glm::vec3(0, 0, 0);
	this->nameTexture = nullptr;
	this->numberTotalFaces = 0;
	this->textureItinitialized = false;
	this->rootListInitialized = false;
}

void GroupFace::setParameters(int Ns, glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, char * nameTexture, char* pathFolder)
{
	this->Ns = Ns;
	this->Ka = Ka;
	this->Kd = Kd;
	this->Ks = Ks;
	/*
	printf("\nNs(%), Ka(%f, %f, %f), Kd((%f, %f, %f), Ks((%f, %f, %f)", this->Ns, this->Ka[0], this->Ka[0], this->Ka[0], 
																		this->Kd[0], this->Kd[1], this->Kd[2], 
																		this->Ks[0], this->Ks[1], this->Ks[2] );
	*/
	// name tex
	std::string strNameTexture = "";
	strNameTexture += nameTexture;
	this->nameTexture = new char[strNameTexture.size() + 1];
	for (int ichar = 0; ichar < (int)strNameTexture.size(); ichar++) {
		this->nameTexture[ichar] = strNameTexture.at(ichar);
	}
	this->nameTexture[strNameTexture.size()] = '\0';

	// path folder
	std::string strPath = "";
	strPath += pathFolder;
	this->pathFolder = new char[strPath.size() + 1];
	for (int ichar = 0; ichar < strPath.size(); ichar++) {
		this->pathFolder[ichar] = strPath.at(ichar);
	}
	this->pathFolder[strPath.size()] = '\0';
}

void GroupFace::setNameGroup(char * nameGroup)
{
	std::string strNameGroup = "";
	strNameGroup += nameGroup;
	this->nameGroup = new char[strNameGroup.size() + 1];
	for (int ichar = 0; ichar < strNameGroup.size(); ichar++) {
		this->nameGroup[ichar] = strNameGroup.at(ichar);
	}
	this->nameGroup[strNameGroup.size()] = '\0';
}

void GroupFace::addFace(Face faceToAdd, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals)
{
	this->numberTotalFaces++;
	this->listFaces.push_back(faceToAdd);
	if (this->numberTotalFaces % 5000 == 0) {
		//printf("\nTime to add 1000 faces : "); ChronoTime::printTimeSince(); // TIME
		printf("\nCreateList (nb faces %i)", this->numberTotalFaces);
		createDrawListFromListFaces(vertices, textures, normals);
		//printf("\nTime to create List to draw : "); ChronoTime::printTimeSince(); // TIME
	}
}

bool GroupFace::compareNameGroup(char * nameGroup)
{
	bool equals = false;
	//printf("\nComparison (%s) and (%s)", this->nameGroup, nameGroup);
	if (!strcmp((this->nameGroup), nameGroup)) {
		//printf(" OK");
		equals = true;
	}
	return equals;
}

// !!! CHANGE
void GroupFace::createListToDraw(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals)
{
	if (!this->textureItinitialized) {
		createTexture();
	}
	// Check if we have some vertex left in list of Face, if yes, create a new list
	if (!this->listFaces.empty()) {
		//printf("\nCreateListToDraw, still faces in list : ");
		createDrawListFromListFaces(vertices, textures, normals);
		//printf("OK");
	}
	// creating a display list
	this->IDListRoot = glGenLists(1);
	// compiling the display list
	glNewList(IDListRoot, GL_COMPILE);
		setMaterial();
		glBindTexture(GL_TEXTURE_2D, this->IDTexture);
		// Draw
		for (int i = 0; i < this->IDlists.size(); i++) {
			glCallList(IDlists[i]);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();
	this->rootListInitialized = true;
}


void GroupFace::draw()
{
	if (this->rootListInitialized) {
		glCallList(this->IDListRoot);
	}
	else {
		printf("\nProblem : Group \"%s\" not drawn, need to call createDrawListFromListFaces(...)", this->nameGroup);
	}
}

void GroupFace::sayHello()
{
	printf("\nHello : my number of lists is %i", this->IDlists.size());
}


void GroupFace::drawSquareTexturedTest()
{
	float size = 2;
	//setMaterial();
	printf("\nTest Draw : %i", this->IDTexture);
	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, this->IDTexture);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f + size, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f + size, size, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, size, 0.0f);
		glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}


void GroupFace::drawGroup(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals)
{
	int index;
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
	int sizeList = this->listFaces.size();
	//printf("\nSizeList : %i", sizeList);
	for (int i = 0; i < sizeList; i++) {
		//printf("\nNEW FACE (%i)----------- ", i);
		glBegin(GL_POLYGON);
			for (int j = 0; j < this->listFaces[i].nbVertices ; j++) {
				//printf("start : %i/%i", j, this->listFaces[i].nbVertices-1);
				index = this->listFaces[i].indexV[j];
				//printf(" index %i ? ", index);
				if (index != -1) {
					position = vertices[index];
					//printf("pOK, ");

					if (this->listFaces[i].haveNormal) {
						index = this->listFaces[i].indexN[j];
						normal = normals[index];
						glNormal3f(normal[0], normal[1], normal[2]);
						//printf("nOK, ");
					}

					if (this->listFaces[i].haveTexture) {
						index = this->listFaces[i].indexT[j];
						texCoord = textures[index];
						glTexCoord2f(texCoord[0], texCoord[1]);
						//printf("tOK, ");
						//printf("\nTEX coord at index %i : (%f, %f)", index, texCoord[0], texCoord[1]);
					}

					// Vertex + tex + normal
					glVertex3f(position[0], position[1], position[2]);
				}
			}
		glEnd();
		//printf("\n");
	}
}


void GroupFace::setMaterial()
{
	// ET DE 1
	float *Ka, *Kd, *Ks;
	Ka = &(this->Ka.x);
	Kd = &(this->Kd.x);
	Ks = &(this->Ks.x);
	//printf("\nK values : %f, rgb(%f,%f,%f), %f, %i", Ka[0], Kd[0], Kd[1], Kd[2], Ks[0], this->Ns);
	glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);
	glMaterialiv(GL_FRONT, GL_SHININESS, &(this->Ns));
}


void GroupFace::createTexture()
{
	// Load file and decode image.
	std::string strPathFolder = this->pathFolder, strTextName = this->nameTexture;
	//printf("\nName texture : %s", this->nameTexture);
	std::string stringPathfinal = ""; 
	stringPathfinal += strPathFolder; stringPathfinal += strTextName;
	// load an image file directly as a new OpenGL texture
	//printf("\n===> Load texture : %s", stringPathfinal.c_str());
	
	this->IDTexture = LoaderTextureJPG::createNewTexture(stringPathfinal.c_str(), GL_REPEAT, GL_NEAREST);
	this->textureItinitialized = true;
}


void GroupFace::createDrawListFromListFaces(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textures, std::vector<glm::vec3> &normals)
{
	//printf("\nCreateDrawListFromListFaces : %i elements", this->listFaces.size());
	// Create a list
	int IDList = glGenLists(1);
	//printf("\n   GenList "); ChronoTime::printTimeSince(); // TIME
	this->IDlists.push_back(IDList);
	//printf("\n   pushList "); ChronoTime::printTimeSince(); // TIME
	glNewList(IDList, GL_COMPILE);
		drawGroup(vertices, textures, normals);
	glEndList();
	//printf("\n   DrawGroup "); ChronoTime::printTimeSince(); // TIME
	// empty the vector
	this->listFaces.clear();
	//printf("\n   Clear "); ChronoTime::printTimeSince(); // TIME
}
