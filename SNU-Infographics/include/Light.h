#pragma once


#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"
#include <../libraries\glm\glm\gtc\matrix_transform.hpp>

#include <iostream>


class Light
{
public:
	struct LightFormatShader {
		glm::vec4 position;
		float attenuation;	// Attenuation of the light between [0,X], depending on distance. 0 Means no attenuation in f° of distance.
		float intensity;
		glm::vec3 colorLight;
		float ambientValue;	// Between 0 and 1. AmbientLight = DiffuseLight * ambientValue.
		int indexLight;
		glm::mat4x4 MVPmatrix;
		// our area light will be a stick of length = distance( position - positionRight );
		int isAreaLight;
		int numberDivisionLightArea;
		glm::vec4 positionRight;
		// shadow
		int hasShadow;
	};

	// Position of camera and her index, ps : index from 0 to 5 (add more if needed in this function)
	Light(glm::vec4 posLight, int indexLight, float intensity, float attenuation);

	// Implementation of glRender
	void glRender();
	void updateShadowMap(void(*functocall)());
	void drawPositionLight(float r, float g, float b, float size);
	void drawSquareTextured();

	GLuint getIDTextureDepth();

	// Call enable or disable light
	void stateLight(bool activated);
	void inverseStateLight();
	bool getStateLight();

	// ------ Setter ------ 
	void setPosLight(glm::vec3 posLight);
	// Colors here can be between [0,1], or more if we want a very strong intensity.
	void setLightColor(glm::vec3 lightColor, float ambientValue, float alpha);	// AmbientValue [0,1] = % color in ambient. Shininess 0 = no, to 50+. Alpha, here no blending so ok.
	void setStateShadow(int hasShadow);
	void setStateAreaLight(int isAreaLight, int numberDivisionLightArea, glm::vec3 posRight);
	// !!! To implement
	void setIntensity(float intensity);
	float setAttenuation(float attenuation);


	glm::mat4x4 getMatrix_Proj_ModelView();


	float* getPtrPosLight();

	LightFormatShader getLightFormatShader();

	GLfloat posLight[4];

	float nearPlan = 0.1f;
	float farPlan = 30.0f;


private:
	// Variables
	GLenum enumLight;
	GLfloat lightColor[4];
	float intensity;
	float attenuation;
	float ambientValue;

	float angleView = 90.0f;
	
	int hasShadow;
	int isAreaLight;
	int numberDivisionLightArea;
	glm::vec4 posRightAreaLight;

/*	
	float angleView = 50.0f;
	float nearPlan = 0.1f;
	float farPlan = 90.0f;
*/	

	// ShadowMap
	const int sizeTexture = 1024;
	GLuint fboLightView;
	GLuint depthTexture;

	glm::mat4x4 proj_ModelView_matrix;

	glm::mat4x4 get_projectionMatrix();
	glm::mat4x4 get_modelViewMatrix();

};

