// project3_glslShadow.cpp : Defines the entry point for the console application.
//

#define GLEW_STATIC

#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"
#include "../libraries\glm\glm\gtc\type_ptr.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "ManagerPickable.h"
#include "Camera.h"
#include "Light.h"
#include "LoaderShader.h"
#include "LoaderOBJ.h"
#include "SOIL.h"
#include "GroundScene.h"
#include "CubeMetal.h"

int IDtext;
int testText();
void drawTextSquare(int IDtexture);

//=====================================================================================================
//			Variables
//=====================================================================================================
int widthViewPort = 1280;
int heightViewPort = 760;

Camera * _myCamera;
float nearPlan = 0.1f, farPlan = 90.0f;
float scaleModel = 0.1f;
glm::mat4 matrix_identity = glm::mat4(1.0f);
glm::mat4 matrix_proj, matrix_view;

bool translateCamera = false;
bool rotateCamera = false;
int xOri, yOri;

glm::mat4x4 scaleMatrix( scaleModel,0,0,0, 0,scaleModel,0,0, 0,0,scaleModel,0, 0,0,0,1  );

// Variables for animation
float timeAnimation;

// lights : 2 point and 1 directional (to implement in glsl)
float radiusLightBox = 0.3f;
Light* lightSelected;
// points
Light* pLight1;
Light* pLight2;
// directional
Light* dLight3;
// Area light
Light* aLight4;
// Serie of light (area light with less division)
Light* sLight5;
// List of lights formated for shader
std::vector<Light::LightFormatShader> myListFormatedLight;
// Matrices proj*ModelView
glm::mat4x4 matLight1, matLight2, matLight3;

// Program shader (for depth)
GLuint programDepth = 0, programLight = 0, vertShader = 0, fragShader = 0;
// Path to shader for Depth
char* pathVertexShaderDepth = "Shaders/VertexShader_seeDepthText.txt";
char* pathFragShaderDepth = "Shaders/FragShader_seeDepthText.txt";
// Path to shader for Light and shadow
char* pathVertexShaderLight = "Shaders/VertexShader_lightAndShadow.txt";
char* pathFragShaderLight = "Shaders/FragShader_lightAndShadow.txt";
// code
GLchar* vertexShaderCode;	unsigned long lengthVert = 0;
GLchar* fragShaderCode; unsigned long lengthFrag = 0;
//Position of variables in program
int posNearProgram = 0; int posFarProgram = 0;
int numberLightsProgram = 0;
int nbVariables = 11;
std::vector<int*> posLightProgram; // 11 variables :	position, attenuation, intensity, color, ambientValue, 
								   //					indexLight, MVPmatrix, isAreaLight, numberDivisionLightArea, positionRight, hasShadow.

int vprog_mat4_MVPmatrix;
int vprog_mat4_InverseModelMatrix;
int fprog_vec3_cameraPosition, fprog_materialTex, fprog_materialShininess;
int fprog_shadowMap[3];

// sphere test
glm::vec3 posTest(0.0f, 0.0f, 0.0f);
float radiusTest = 0.2f;

// ManagerPickable
ManagerPickable * _myManagerPickable;

// Lists for drawing model .OBJ
std::vector<glm::vec3> listVertices, listNormals;
std::vector<glm::vec2> listTextures;
std::vector<GroupFace> listGroups;
std::string pathAllObjects = "Objects/" ;

/*
std::string nameFolderObj = "BrandenburgGate/";
std::string nameObj = "BrandenburgGate.obj";
*/

std::string nameFolderObj = "Farmhouse/";
std::string nameObj = "farmhouse_obj.obj";

// My wall
CubeMetal* myCube;
float testShininess = 1.0f;
std::string pathTextureWall = "Objects/metalTexture2.jpg";

// My ground 
std::string pathTextureGround = "Objects/groundTexture.jpg";
GroundScene * _myGround;


//=====================================================================================================
//			Declaration of functions
//=====================================================================================================

void draw_axis_xyz(float lengthAxis);
void drawScene();
void drawOverlayImageDepth(Light* lightSelected);	// 1 and 2 are point, and 3 directional

// Not done
GLuint gBuffer;
void createGBuffer();

glm::mat4x4 getMatrixMVP();

// Light
void initLight();
void switchLightSource();
void printStateLight();
void updateListLightsForShader();
void updateMVPmatrixShader();
void setVariablesUniformShader();

// Material
void initMaterialTeaPot();

// FBO
void initFBO();
void setSceneIntoFBO();
void drawScene();

void drawSceneGLSL();

// GLSL
void createProgram();

void printMatrix(glm::mat4x4 mat);

void checkLight0GLSL() {
	//float value_attenuation;
	//printf("\nTest location in prog 1 : posNearProg(%i), posAttenuationLight0(%i)", posNearProgram, posLightProgram[0][1]);
	//glGetUniformfv(programLight, posLightProgram[0][2], &value_attenuation);
	//printf("\nValue attenuation : %f", value_attenuation);
	if (myListFormatedLight.size() > 0) {
		int nLights; glGetUniformiv(programLight, numberLightsProgram, &nLights);
		glm::vec4 position;	glGetUniformfv(programLight, posLightProgram[0][0], (float*)&position);
		float attenuation; glGetUniformfv(programLight, posLightProgram[0][1], &attenuation);
		float intensity; glGetUniformfv(programLight, posLightProgram[0][2], &intensity);
		glm::vec3 colorLight; glGetUniformfv(programLight, posLightProgram[0][3], (float*)&colorLight);
		float ambientValue; glGetUniformfv(programLight, posLightProgram[0][4], &ambientValue);
		int indexLight; glGetUniformiv(programLight, posLightProgram[0][5], &indexLight);
		printf("\n ====== nblights(%i) - TEST LIGHT : pos(%f, %f, %f, %f), color(%f,%f,%f), atten(%f), intens(%f), ambient(%f), indexLight(%i)",
			nLights, position.x, position.y, position.z, position[3], colorLight.x, colorLight.y, colorLight.z, attenuation, intensity, ambientValue, indexLight);
	}
	int textureID; glGetUniformiv(programLight, fprog_materialTex, &textureID);
	printf("\nTexture : %i", textureID);
}

//=======================================================================================================
//				RenderScene, Reshape, onMouseButton, onMouseMove, Main
//=======================================================================================================

void renderScene(void)
{
	//printf("\n---- Rendering");
	pLight1->glRender();
	pLight2->glRender();
	dLight3->glRender();
	aLight4->glRender();
	sLight5->glRender();

	// Update shadow map
	pLight1->updateShadowMap(drawScene);
	pLight2->updateShadowMap(drawScene);
	dLight3->updateShadowMap(drawScene);


	// - On screen
	_myCamera->glRender();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_axis_xyz(5.0f);

	//_myCamera->drawTarget(0.5f);
	_myManagerPickable->drawLinesToOriginObjectPicked();

	// Activate lights
	pLight1->drawPositionLight(1.0f, 0.8f, 0.8f, radiusLightBox);
	pLight2->drawPositionLight(1.0f, 0.5f, 0.0f, radiusLightBox);
	dLight3->drawPositionLight(0.5f, 0.5f, 1.0f, radiusLightBox);
	aLight4->drawPositionLight(0.1f, 0.1f, 1.0f, radiusLightBox);
	sLight5->drawPositionLight(0.1f, 0.8f, 0.2f, radiusLightBox);

	glPushMatrix();
		glColor3f(1.0f, 1.0f, 0.0f);
		glTranslatef(lightSelected->posLight[0], lightSelected->posLight[1], lightSelected->posLight[2]);
		glutSolidSphere(0.2f, 10, 10);
	glPopMatrix();

	int cptHasShadow = 0;
	for (int i = 0; i < (int)myListFormatedLight.size(); i++) {
		if (myListFormatedLight[i].hasShadow) {
			//printf("\n======= Test 2(%i) 3(%i)", GL_TEXTURE2, GL_TEXTURE3);
			glActiveTexture(GL_TEXTURE2 + cptHasShadow);
			cptHasShadow++;
			//printf("\nActive text n° %i", cptHasShadow);
			//printf("\nFor active tex : %i", GL_TEXTURE2 + i);
			switch (myListFormatedLight[i].indexLight) {
				case 0:
					//printf("\nLIGHT NUMBER 0 : ID %i", pLight1->getIDTextureDepth());
					glBindTexture(GL_TEXTURE_2D, pLight1->getIDTextureDepth());
					break;
				case 1:
					//printf("\nLIGHT NUMBER 1 : ID %i", pLight2->getIDTextureDepth());
					glBindTexture(GL_TEXTURE_2D, pLight2->getIDTextureDepth());
					break;
				case 2:
					//printf("\nLIGHT NUMBER 2 : ID %i", dLight3->getIDTextureDepth());
					glBindTexture(GL_TEXTURE_2D, dLight3->getIDTextureDepth());
					break;
			}
		}
	}

	glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHTING);
				drawSceneGLSL();
			glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);

	// Draw the overlay image
	drawOverlayImageDepth(lightSelected);

	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	widthViewPort = w;
	heightViewPort = h;
	glViewport(0, 0, w, h);
	_myCamera->glRender();
	_myManagerPickable->updateInverseProjViewMatrix();
}


void onMouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		// If no object is under the mouse when we click, then we rotate camera
		if (state == GLUT_DOWN) {
			if (_myManagerPickable->tryToPickObject(x, y) == -1) {
				xOri = x; yOri = y;
				rotateCamera = true;
			}
		}
		else if (state == GLUT_UP) {
			_myManagerPickable->releaseobject();
			if (rotateCamera) {
				rotateCamera = false;
				_myCamera->updateNormalVectors();
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			xOri = x; yOri = y;
			translateCamera = true;
		}
		else if (state == GLUT_UP) {
			translateCamera = false;
		}
	}
	glutPostRedisplay();
}


void onMouseMove(int x, int y) {
	bool objectMoved = false;
	if (rotateCamera) {
		//printf("\nRotation");
		_myCamera->rotation_camera((float)x, (float)y, (float)xOri, (float)yOri);
		xOri = x; yOri = y;
	}
	else if (translateCamera) {
		//printf("\ntranslation");
		_myCamera->translation_camera((float)x, (float)y, (float)xOri, (float)yOri);
		xOri = x; yOri = y;
	}
	_myManagerPickable->updateInverseProjViewMatrix();
	objectMoved = _myManagerPickable->moveObjectPicked(x, y);
	if (objectMoved) {
		// In our case, only lights can be moved, so we update our list for shader.
		updateListLightsForShader();
	}
	glutPostRedisplay();
}

void MyKeyboardFunc(unsigned char Key, int x, int y)
{
	switch (Key)
	{
		case '+':
			_myCamera->zoom_camera(true);
			glutPostRedisplay();
			break;
		case '-':
			_myCamera->zoom_camera(false);
			glutPostRedisplay();
			break;
		case 'd':
			switchLightSource();
			break;
		case '1':
			pLight1->inverseStateLight();
			updateListLightsForShader();
			printStateLight();
			break;
		case '2':
			pLight2->inverseStateLight();
			updateListLightsForShader();
			printStateLight();
			break;
		case '3':
			dLight3->inverseStateLight();
			updateListLightsForShader();
			printStateLight();
			break;
		case '4':
			aLight4->inverseStateLight();
			updateListLightsForShader();
			printStateLight();
			break;
		case '5':
			sLight5->inverseStateLight();
			updateListLightsForShader();
			printStateLight();
			break;
		case 's':
			myCube->changeShininess(testShininess);
			testShininess += 1.0f;
			printf("\nShininess : %f", testShininess);
			break;
	}
	glutPostRedisplay();
}

void idle() {
	int t;
	/* Delta time in seconds. */
	t = glutGet(GLUT_ELAPSED_TIME);
	
	if ((float)(t - timeAnimation) / 1000.0f > 0.005f) {
		// Action to move object ???
		glutPostRedisplay();
	}
	
}


void OnExit()
{
	// Do something ?
}


int main(int argc, char **argv)
{
	printf("\n----------- Initializing openGL -----------\n");

	//glm::vec3 posCamera(-2.0f, 4.0f, 4.0f);

	glm::vec3 posCamera(10, 10, 10);
	glm::vec3 targetCamera(0, 0, 0);
	glm::vec3 upCamera(0, 1, 0);
	_myCamera = new Camera(posCamera, targetCamera, upCamera, 0.001f, nearPlan, farPlan);

	// Initialization for OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(widthViewPort, heightViewPort);
	glutCreateWindow("Window : MoCap");
	glewInit();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMove);
	glutKeyboardFunc(MyKeyboardFunc);
	//glutIdleFunc(idle);
	printf("\n----------- Start drawing (main loop) -----------");
	timeAnimation = (float)glutGet(GLUT_ELAPSED_TIME);

	// Initilization of others parameters
	_myCamera->glRender();	// set projection and modelView matrices
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// creation of lights and programs / shaders
	initLight();
	printf("\n\n====================\nCreation of programs with shaders");
	createProgram();

	// Manager pick
	_myManagerPickable = new ManagerPickable();
	_myManagerPickable->updateNearAndFar(nearPlan, farPlan);
	_myManagerPickable->updateInverseProjViewMatrix();

	// Lights
	int index = _myManagerPickable->addObjectPickable(radiusLightBox, (glm::vec3*)pLight1->getPtrPosLight());
	//printf("\nTest index added : %i", index);
	index = _myManagerPickable->addObjectPickable(radiusLightBox, (glm::vec3*)pLight2->getPtrPosLight());
	//printf("\nTest index added : %i", index);
	index = _myManagerPickable->addObjectPickable(radiusLightBox, (glm::vec3*)dLight3->getPtrPosLight());
	//printf("\nTest index added : %i", index);
	index = _myManagerPickable->addObjectPickable(radiusLightBox, (glm::vec3*)aLight4->getPtrPosLight());
	index = _myManagerPickable->addObjectPickable(radiusLightBox, (glm::vec3*)sLight5->getPtrPosLight());
	
	// cube
	//index = _myManagerPickable->addObjectPickable(myCube.getRadiusCube(), (glm::vec3*)myCube.getPtrCenterCube());
	//printf("\nIndex cube : %i", index);

	// Create model
	printf("\n\n====================");
	LoaderOBJ myLoaderObj;
	std::string pathObj = pathAllObjects+nameFolderObj;
	myLoaderObj.loadObj(pathObj.c_str(), nameObj.c_str(), listVertices, listTextures, listNormals, listGroups);
	for (int i = 0; i < (int)listGroups.size(); i++) {
		//printf("\n GROUP : %i, size list : %i", i, listGroups.size());
		listGroups[i].createListToDraw(listVertices, listTextures, listNormals);
	}
	printf("\n====================\n");

	_myGround = new GroundScene(pathTextureGround.c_str(), 20.0f, 10);
	myCube = new CubeMetal(pathTextureWall.c_str(), 20.0f, glm::vec3(0, 10, -5), true, 1);

	//glShadeModel(GL_SMOOTH);

	printf("\nCommands : \n- 'd' to switch light point of view (Light selected with yellow sphere.)");
	printf("\n- '1', '2', '3', '4' or '5' to active/desactivate lights");
	printf("\nPS : Light 1 and 2 are point lights, 3 is directional, 4 is an area light and 5 a serie of several lights.");
	printf("\n- Right Click to translate camera, and Left click to rotate.");
	printf("\n- Left click on lights (boxes) to move them. PS : blue box is directional light.");

	// Loop
	glutMainLoop();

	OnExit();

	return 0;
}


// =====================================================================================================

void draw_axis_xyz(float lengthAxis) {
	glBegin(GL_LINES);
		// Axis x : Red
		glColor3ub(150, 0, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(lengthAxis, 0.0f, 0.0f);
		// Axis y : Green
		glColor3ub(0, 150, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, lengthAxis, 0.0f);
		// Axis z : Blue
		glColor3ub(0, 0, 150);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, lengthAxis);
	glEnd();
}


void drawScene() {
	// Draw ground
	_myGround->drawGround();
	myCube->drawCube();
	// Draw Model
	glPushMatrix();
		glScalef(scaleModel, scaleModel, scaleModel);
		for (int i = 0; i < (int)listGroups.size(); i++) {
			listGroups[i].draw();
		}
	glPopMatrix();
}


void drawSceneGLSL() {

	glUseProgram(programLight);
		setVariablesUniformShader();
		glUniformMatrix4fv(vprog_mat4_InverseModelMatrix, 1, false, glm::value_ptr(matrix_identity));
		// Draw ground
		glActiveTexture(GL_TEXTURE0);
		_myGround->drawGround();
		myCube->drawCube();
		// Draw Model
		glPushMatrix();
			glScalef(scaleModel, scaleModel, scaleModel);
			glUniformMatrix4fv(vprog_mat4_InverseModelMatrix, 1, false, glm::value_ptr(glm::inverse(scaleMatrix)));
			updateMVPmatrixShader();
			for (int nLight = 0; nLight < (int)myListFormatedLight.size(); nLight++) {
				glm::mat4 MVPlightModified = myListFormatedLight[nLight].MVPmatrix * scaleMatrix;
				glUniformMatrix4fv(posLightProgram[nLight][6], 1, false, glm::value_ptr(MVPlightModified));
			}
			for (int i = 0; i < (int)listGroups.size(); i++) {
				listGroups[i].draw();
			}
		glPopMatrix();
	glUseProgram(0);
}


void initLight() {

	printf("\nInit lights : ");
	// point light 1
	pLight1 = new Light(glm::vec4(-2.0f, 6.0f, 6.0f, 1.0f), 0, 2.0f, 0.01f);	// W at 1 = point
	pLight1->setLightColor(glm::vec3(1, 1, 1), 0.0f, 1.0f);
	pLight1->setStateAreaLight(0, 0, glm::vec3(0,0,0));
	pLight1->setStateShadow(1);
	
	// point light 2
	pLight2 = new Light(glm::vec4(-4.0f, 4.0f, -4.0f, 1.0f), 1, 2.0f, 0.01f);	// W at 1 = point
	pLight2->setLightColor(glm::vec3(1.0f, 0.2f, 1.0f), 0.0f, 1.0f);
	pLight2->setStateAreaLight(0, 0, glm::vec3(0, 0, 0));
	pLight2->setStateShadow(1);

	// directional light 3
	dLight3 = new Light(glm::vec4(5.0f, 5.0f, 0.0f, 0.0f), 2, 0.8f, 0);// W at 0 = directional, value of attenuation not needed because will be 0.
	dLight3->setLightColor(glm::vec3(1.0f, 1.0f, 1.0f), 0.15f, 1.0f);
	dLight3->setStateAreaLight(0, 0, glm::vec3(0, 0, 0));
	dLight3->setStateShadow(1);

	// Area light 4
	aLight4 = new Light(glm::vec4(-2.0f, 3.0f, 3.0f, 1.0f), 3, 3.0f, 0.2f);
	aLight4->setLightColor(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f);
	aLight4->setStateAreaLight(1, 40, glm::vec3(6.0f, 0.0f, 0.0f));
	aLight4->setStateShadow(0);

	// Serie of light 5
	sLight5 = new Light(glm::vec4(-10.0f, 0.3f, -4.0f, 1.0f), 4, 15.0f, 0.2f);
	sLight5->setLightColor(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f);
	sLight5->setStateAreaLight(1, 10, glm::vec3(20.0f, 0.0f, 0.0f));
	sLight5->setStateShadow(0);

	pLight1->stateLight(true);
	pLight2->stateLight(true);
	dLight3->stateLight(true);
	aLight4->stateLight(true);
	sLight5->stateLight(true);

	lightSelected = pLight1;
	printStateLight();
	updateListLightsForShader();
}

void switchLightSource() {
	if (lightSelected == pLight1) {
		lightSelected = pLight2;
	} 
	else if (lightSelected == pLight2) {
		lightSelected = dLight3;
	}
	else {
		lightSelected = pLight1;
	}
}

void printStateLight() {
	//printf("\n================= Click on 1,2,3 to activate or desactivate a light");
	printf("\n----------------------------");
	printf("\nLight Point 1 : %d", glIsEnabled(GL_LIGHT0));
	printf("\nLight Point 2 : %d", glIsEnabled(GL_LIGHT1));
	printf("\nLight Direc 3 : %d", glIsEnabled(GL_LIGHT2));
	printf("\nLight Area  4 : %d", glIsEnabled(GL_LIGHT3));
	printf("\nLight Serie 5 : %d", glIsEnabled(GL_LIGHT4));
}

void updateListLightsForShader() {
	std::vector<Light::LightFormatShader> listLights;
	//printf("\nUpdate List of lights for shader");
	if (pLight1->getStateLight()) {
		listLights.push_back(pLight1->getLightFormatShader() );
	}
	if (pLight2->getStateLight()) {
		listLights.push_back(pLight2->getLightFormatShader());
	}
	if (dLight3->getStateLight()) {
		listLights.push_back(dLight3->getLightFormatShader());
	}
	if (aLight4->getStateLight()) {
		listLights.push_back(aLight4->getLightFormatShader());
	}
	if (sLight5->getStateLight()) {
		listLights.push_back(sLight5->getLightFormatShader());
	}
	//printf("\nThere are %i lights in list for shader", listLights.size());
	myListFormatedLight = listLights;

}


void updateMVPmatrixShader() {
	glm::mat4x4 MVPmatrix = getMatrixMVP();
	glUniformMatrix4fv(vprog_mat4_MVPmatrix, 1, false, glm::value_ptr(MVPmatrix));
}


void setVariablesUniformShader() {
	/*
		vec4 position;
	   float attenuation;
	   float intensity
	   vec3 colorLight;
	   float ambientValue;

	   and posLightProgram : 5 variables -> position, attenuation, intensity, color, ambientValue.
	*/

	glUniform1i(numberLightsProgram, myListFormatedLight.size());
	//printf("\nPos light 0 : %f, %f, %f and index in program %i", myListFormatedLight[0].position.x, myListFormatedLight[0].position.y, myListFormatedLight[0].position.z, posLightProgram[0][0]);
	int cptHasShadow = 0;
	for (int nLight = 0; nLight < (int)myListFormatedLight.size(); nLight++) {
		glUniform4fv(posLightProgram[nLight][0], 1, glm::value_ptr(myListFormatedLight[nLight].position));
		//printf("\nSet var unif shader : %f, %f, %f", myListFormatedLight[nLight].position[0], myListFormatedLight[nLight].position[1], myListFormatedLight[nLight].position[2]);
		glUniform1f(posLightProgram[nLight][1], myListFormatedLight[nLight].attenuation);
		glUniform1f(posLightProgram[nLight][2], myListFormatedLight[nLight].intensity);
		glUniform3fv(posLightProgram[nLight][3], 1, glm::value_ptr(myListFormatedLight[nLight].colorLight));
		glUniform1f(posLightProgram[nLight][4], myListFormatedLight[nLight].ambientValue);
		glUniform1i(posLightProgram[nLight][5], myListFormatedLight[nLight].indexLight);
		glUniformMatrix4fv(posLightProgram[nLight][6], 1, false, glm::value_ptr(myListFormatedLight[nLight].MVPmatrix));
		glUniform1i(posLightProgram[nLight][7], myListFormatedLight[nLight].isAreaLight);
		glUniform1i(posLightProgram[nLight][8], myListFormatedLight[nLight].numberDivisionLightArea);
		glUniform4fv(posLightProgram[nLight][9], 1, glm::value_ptr(myListFormatedLight[nLight].positionRight));
		glUniform1i(posLightProgram[nLight][10], myListFormatedLight[nLight].hasShadow);
		if (myListFormatedLight[nLight].hasShadow) {
			glUniform1i(fprog_shadowMap[nLight], 2 + cptHasShadow);
			cptHasShadow++;
		}
	}
	//printf("\n=======================");
	//checkLight0GLSL();

	/*
	// Set these parameters
	int vprog_mat4_camera, vfprog_mat4_model;	// projection*view
	int fprog_vec3_cameraPosition, fprog_materialTex, fprog_materialShininess;
	*/
	updateMVPmatrixShader();
	glm::vec3 posCamera = _myCamera->getPosCamera();
	glUniform3fv(fprog_vec3_cameraPosition, 1, glm::value_ptr(posCamera));
	glUniform1i(fprog_materialTex, 0);
	

	/*
	glm::vec3 posCamera(-2.0f, 4.0f, 4.0f);

	//glm::vec3 posCamera(10, 10, 10);
	glm::vec3 targetCamera(0, 0, 0);
	glm::vec3 upCamera(0, 1, 0);
	_myCamera = new Camera(posCamera, targetCamera, upCamera, 0.001f, nearPlan, farPlan);
	*/

	//float value_attenuation;
	//printf("\nTest location in prog 1 : posNearProg(%i), posAttenuationLight0(%i)", posNearProgram, posLightProgram[0][1]);
	//glGetUniformfv(programLight, posLightProgram[0][2], &value_attenuation);
	//printf("\nValue attenuation : %f", value_attenuation);
}



void createProgram()
{
	// print version
	char* version = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("\n____\nVersion : %s\n____", version);

	// - 1 - Shader for Depth Mapping ---
	// Load shaders for depth
	LoadShader loaderShader;
	loaderShader.loadshader(pathVertexShaderDepth, &vertexShaderCode, &lengthVert);
	loaderShader.loadshader(pathFragShaderDepth, &fragShaderCode, &lengthFrag);
	// Create shaders
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Specify source(vertShader, 1, &vertexShaderCode, (GLint*)(&lengthVert));
	glShaderSource(vertShader, 1, &vertexShaderCode, (GLint*)(&lengthFrag));
	glShaderSource(fragShader, 1, &fragShaderCode, (GLint*)(&lengthFrag));
	/*
	printf("\n--------------------------------------------------");
	printf("\nMy progr vert : \n%s", vertexShaderCode);
	printf("\n--------------------------------------------------");
	printf("\nMy progr pix : \n%s", pixelShaderCode);
	*/
	// Compile
	glCompileShader(vertShader);
	glCompileShader(fragShader);

	// Create program Depth
	programDepth = glCreateProgram();
	// Attach them to program
	glAttachShader(programDepth, vertShader);
	glAttachShader(programDepth, fragShader);
	// Link program
	glLinkProgram(programDepth);

	// Check if linking is a success
	GLint linked;
	glGetProgramiv(programDepth, GL_LINK_STATUS, &linked);
	if (linked)
	{
		printf("\n=> Program for -Drawing depth- Linked with success\n");
	}
	else {
		printf("\n");
		GLint maxLength = 0;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);
		//The maxLength includes the NULL character
		char * infoLog1 = new char[maxLength];
		glGetShaderInfoLog(vertShader, maxLength, &maxLength, infoLog1);
		printf("\nError ? %s", infoLog1);

		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);
		//The maxLength includes the NULL character
		char * infoLog2 = new char[maxLength];
		glGetShaderInfoLog(fragShader, maxLength, &maxLength, infoLog2);
		printf("\nError ? %s", infoLog2);

		printf("\n\nERROR !!! Program not linked\n");
		Sleep(2000);
	}

	// Get position of variable near and far plan in program
	posNearProgram = glGetUniformLocation(programDepth, "near");
	posFarProgram = glGetUniformLocation(programDepth, "far");
	//printf("\nPos near : %i and far : %i", posNearProgram, posFarProgram);



	// - 2 - Shader for Light and shadow ---
	free(vertexShaderCode);
	free(fragShaderCode);
	// Load shaders for depth
	loaderShader.loadshader(pathVertexShaderLight, &vertexShaderCode, &lengthVert);
	loaderShader.loadshader(pathFragShaderLight, &fragShaderCode, &lengthFrag);
	// Create shaders
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Specify source(vertShader, 1, &vertexShaderCode, (GLint*)(&lengthVert));
	glShaderSource(vertShader, 1, &vertexShaderCode, (GLint*)(&lengthFrag));
	glShaderSource(fragShader, 1, &fragShaderCode, (GLint*)(&lengthFrag));

	// Compile
	glCompileShader(vertShader);
	glCompileShader(fragShader);

	// Create program Depth
	programLight = glCreateProgram();
	// Attach them to program
	glAttachShader(programLight, vertShader);
	glAttachShader(programLight, fragShader);
	// Link program
	glLinkProgram(programLight);

	// Check if linking is a success
	glGetProgramiv(programLight, GL_LINK_STATUS, &linked);
	if (linked)
	{
		printf("\n=> Program for -Light_and_shadow- Linked with success");
	}
	else {
		printf("\n");
		GLint maxLength = 0;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);
		//The maxLength includes the NULL character
		char * infoLog1 = new char[maxLength];
		glGetShaderInfoLog(vertShader, maxLength, &maxLength, infoLog1);
		printf("\nError ? %s", infoLog1);

		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);
		//The maxLength includes the NULL character
		char * infoLog2 = new char[maxLength];
		glGetShaderInfoLog(fragShader, maxLength, &maxLength, infoLog2);
		printf("\nError ? %s", infoLog2);

		printf("\n\nERROR !!! Program not linked\n");
		Sleep(2000);
	}

	// Get position of variable in program
	//printf("\nIn program light : %i", programLight);
	int testN = glGetUniformLocation(programLight, "listLights[0].position");
	//printf("\nTest position : %i", testN);

	testN = glGetUniformLocation(programLight, "model");
	//printf("\nTest model : %i", testN);

	numberLightsProgram = glGetUniformLocation(programLight, "numLights");
	//printf("\nNb light shader : %i", numberLightsProgram);

	std::string nameVariable = "listLights";
	for (int nlight = 0; nlight < 10; nlight++) {
		int* posVariables = new int[nbVariables];
		//printf("\n----> ");
		std::string numberLight = "[" + std::to_string(nlight); numberLight += "]";
		std::string nameAndNumber = nameVariable + numberLight;
		std::string aux = nameAndNumber + ".position";
		posVariables[0] = glGetUniformLocation(programLight, aux.c_str());
		//printf("\nAux : %s --->", aux.c_str()); printf("0(%i) ", posLightProgram[nlight][0]);
		aux = nameAndNumber + ".attenuation";
		posVariables[1] = glGetUniformLocation(programLight, aux.c_str());
		//printf("\nAux : %s --->", aux.c_str()); printf(" 1(%i) ", posLightProgram[nlight][1]);
		aux = nameAndNumber + ".intensity";
		posVariables[2] = glGetUniformLocation(programLight, aux.c_str());
		//printf("\nAux : %s --->", aux.c_str()); printf(" 2(%i) ", posLightProgram[nlight][2]);
		aux = nameAndNumber + ".colorLight";
		posVariables[3] = glGetUniformLocation(programLight, aux.c_str());
		//printf("\nAux : %s --->", aux.c_str()); printf(" 3(%i) ", posLightProgram[nlight][3]);
		aux = nameAndNumber + ".ambientValue";
		posVariables[4] = glGetUniformLocation(programLight, aux.c_str());
		//printf("\nAux : %s --->", aux.c_str()); printf(" 4(%i) ", posLightProgram[nlight][4]);
		aux = nameAndNumber + ".indexLight";
		posVariables[5] = glGetUniformLocation(programLight, aux.c_str());
		//printf("\nAux : %s --->", aux.c_str()); printf(" 5(%i) ", posLightProgram[nlight][5]);
		aux = nameAndNumber + ".MVPmatrixLight";
		posVariables[6] = glGetUniformLocation(programLight, aux.c_str());
		//printf("\nAux : %s --->", aux.c_str()); printf(" 6(%i) ", posLightProgram[nlight][6]);
		aux = nameAndNumber + ".isAreaLight";
		posVariables[7] = glGetUniformLocation(programLight, aux.c_str());
		aux = nameAndNumber + ".numberDivisionLightArea";
		posVariables[8] = glGetUniformLocation(programLight, aux.c_str());
		aux = nameAndNumber + ".positionRight";
		posVariables[9] = glGetUniformLocation(programLight, aux.c_str());
		aux = nameAndNumber + ".hasShadow";
		posVariables[10] = glGetUniformLocation(programLight, aux.c_str());
		posLightProgram.push_back(posVariables);
	}
	
	/*
	int vprog_mat4_camera, vprog_mat4_model;
	int fprog_mat4_model, fprog_vec3_cameraPosition, fprog_materialTex, fprog_materialShininess;
	*/
	vprog_mat4_MVPmatrix = glGetUniformLocation(programLight, "MVPmatrix");
	vprog_mat4_InverseModelMatrix = glGetUniformLocation(programLight, "InverseModelMatrix");
	fprog_vec3_cameraPosition = glGetUniformLocation(programLight, "cameraPosition");
	fprog_materialTex = glGetUniformLocation(programLight, "materialTex");
	fprog_materialShininess = glGetUniformLocation(programLight, "materialShininess");
	fprog_shadowMap[0] = glGetUniformLocation(programLight, "ShadowMap[0]");
	fprog_shadowMap[1] = glGetUniformLocation(programLight, "ShadowMap[1]");
	fprog_shadowMap[2] = glGetUniformLocation(programLight, "ShadowMap[2]");

	//printf("\nMVP matrix : %i", vprog_mat4_MVPmatrix);
	//printf("\nCam pos : %i", fprog_vec3_cameraPosition);
}


int testText() {
	// Load file and decode image.
	GLint width, height;
	std::string stringPathfinal = "Objects/BrandenburgGate/BronzeRoof1.jpg";
	// load an image file directly as a new OpenGL texture
	printf("\n===> Load texture : %s", stringPathfinal.c_str());
	GLuint tex_2d = SOIL_load_OGL_texture
	(
		stringPathfinal.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB
	);
	int miplevel = 0;

	glBindTexture(GL_TEXTURE_2D, tex_2d);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &height);
		// check for an error during the load process
		if (0 == tex_2d)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	printf("\nText ID : %i", tex_2d);

	// Test print
	glBindTexture(GL_TEXTURE_2D, tex_2d);
		GLfloat* tabPix = new GLfloat[width*height * 3];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, tabPix);
		float max = -1000.0f, min = 1000.0f;
		for (int index = 0; index < width*height - 1; index++) {
			if (tabPix[index] > max) {
				max = tabPix[index];
			}
			if (tabPix[index] < min) {
				min = tabPix[index];
			}
		}
		delete tabPix;
		printf("\n--------- Max (%f) and MIN (%f)", max, min);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex_2d;
}

void drawTextSquare(int IDtexture) {
	float size = 2;
	//setMaterial();
	//printf("\nTest Draw : %i", IDtexture);
	glBindTexture(GL_TEXTURE_2D, IDtexture);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f + size, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f + size, size, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, size, 0.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}


void drawOverlayImageDepth(Light* lightSelected) {
	if (lightSelected != nullptr) {
		float nearP = lightSelected->nearPlan, farP = lightSelected->farPlan;
		if (lightSelected->posLight[3] == 0.0f) {
			// direct light
			nearP = 2.0f;
			farP = 12.0f;
		}
		glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, lightSelected->getIDTextureDepth());
				glUseProgram(programDepth);
					glUniform1f(posNearProgram, nearP);
					glUniform1f(posFarProgram, farP);
					glBegin(GL_QUADS);
						glTexCoord2f(0, 0); glVertex3d(-1.0, -1.0, 0);
						glTexCoord2f(1, 0); glVertex3d(-0.5, -1.0, 0);
						glTexCoord2f(1, 1); glVertex3d(-0.5, -0.5, 0);
						glTexCoord2f(0, 1); glVertex3d(-1.0, -0.5, 0);
					glEnd();
				glUseProgram(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glMatrixMode(GL_PROJECTION);
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
	}
}

void printMatrix(glm::mat4x4 mat) {
	for (int line = 0; line < 4; line++) {
		printf("\n| ");
		for (int col = 0; col < 4; col++) {
			printf(" %f ", mat[line][col]);
		}
		printf("|");
	}
}

glm::mat4x4 get_projectionMatrix() {
	float m[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	glGetFloatv(GL_PROJECTION_MATRIX, m);
	glm::mat4x4 projectionMatrix;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			projectionMatrix[col][row] = m[col * 4 + row];
		}
	}
	return projectionMatrix;
}

glm::mat4x4 get_modelViewMatrix() {
	float m[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glm::mat4x4 modelViewMatrix;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			modelViewMatrix[col][row] = m[col * 4 + row];
		}
	}
	return modelViewMatrix;
}

glm::mat4x4 getMatrixMVP() {

	glm::mat4x4 MVPmatrix = get_projectionMatrix()*get_modelViewMatrix();

	return MVPmatrix;
}


void createGBuffer() {
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	unsigned int gPosition, gNormal, gColorSpec, gAlbedoSpec;

	// - position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, widthViewPort, heightViewPort, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// - normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, widthViewPort, heightViewPort, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// - color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthViewPort, heightViewPort, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
}