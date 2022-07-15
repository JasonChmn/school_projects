#pragma once


#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"

#include <iostream>


class Camera
{
public:
	// speed_camera, value between 0.01 and 1.0
	Camera( glm::vec3 Pos, glm::vec3 Target, glm::vec3 Up, float speed_camera, float nearPlan, float farPlan);
	~Camera();

	// Implementation of glRender
	void glRender();
	void glRenderOrigin();

	void drawTarget(float radius);

	// ------ Setter ------ 
	void setPosCamera(glm::vec3 posCamera);
	void setTargetCamera(glm::vec3 targetCamera);
	void setUpCamera(glm::vec3 upCamera);
	// ------ Getter ------ 
	glm::vec3 getPosCamera();
	glm::vec3 getTargetCamera();
	glm::vec3 getUpCamera();

	glm::mat4x4 getMatrixModelView();
	glm::mat4x4 getMatrixProjection();

	// ------ Actions on camera ------ 
	void rotation_camera(float x, float y, float xOri, float yOri);
	void translation_camera(float x, float y, float xOri, float yOri);
	void zoom_camera(bool ZoomIn);
	void updateNormalVectors();	// To Call everytime we stop an action on Camera (When GLUT UP...)

	// ------ print information ------
	void printParameters();

private:
	// Variables
	glm::vec3 posCamera;	// pos_camera
	glm::vec3 targetCamera;	// pos_target -> where to look
	glm::vec3 upCamera;
	float speedCamera;
	float nearPlan, farPlan;

	glm::vec3 posCameraOrign;
	glm::vec3 targetCameraOrigin;

	float angleHorizontal;
	float angleVertical;
	float lengthPositionToTarget;
	glm::vec3 vectorNormalTop;
	glm::vec3 vectorNormalRight;

	float calculAngleHorizontal(glm::vec3 myVector);
	float calculAngleVertical(glm::vec3 myVector);

	glm::mat4 matrixProjection, matrixModelView;
};

