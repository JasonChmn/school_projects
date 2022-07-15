

# define M_PI          3.141592653589793238462643383279502884L /* pi */

#include "../include/Camera.h"

// =============== Constructor and Destructor ====================================================

Camera::Camera( glm::vec3 Pos, glm::vec3 Target, glm::vec3 Up, float speed_camera, float nearPlan, float farPlan)
{
	this->posCamera = Pos;
	this->posCameraOrign = Pos;
	this->targetCamera = Target;
	this-> targetCameraOrigin = Target;
	this->upCamera = Up;
	this->speedCamera = speed_camera;
	this->nearPlan = nearPlan;
	this->farPlan = farPlan;

	glm::vec3 vectorPosCameraToTarget = this->targetCamera - this->posCamera;
	this->lengthPositionToTarget = glm::length(vectorPosCameraToTarget);
	this->angleHorizontal = calculAngleHorizontal(vectorPosCameraToTarget);
	this->angleVertical = calculAngleVertical(vectorPosCameraToTarget);
	updateNormalVectors();
}


Camera::~Camera()
{
}

// ============== glRender() =====================================================================

void Camera::glRender()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		/*
		GLfloat aspect = (GLfloat)viewport[2] / (GLfloat)viewport[3];
		float value = 10.0f;
		glOrtho(-value, value, -value / aspect, value / aspect, 0.1, 100);
		*/
		gluPerspective(50.0f, (double)(viewport[2]) /(double)(viewport[3]), this->nearPlan, this->farPlan);
		glGetFloatv(GL_PROJECTION, (float*)&this->matrixProjection);
		
	glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(	this->posCamera.x, this->posCamera.y, this->posCamera.z,
					this->targetCamera.x, this->targetCamera.y, this->targetCamera.z,
					this->upCamera.x, this->upCamera.y, this->upCamera.z);
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&this->matrixModelView);
}

void Camera::glRenderOrigin()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0f, (double)(viewport[2]) / (double)(viewport[3]), 0.1f, 15.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(this->posCameraOrign.x, this->posCameraOrign.y, this->posCameraOrign.z,
		this->targetCameraOrigin.x, this->targetCameraOrigin.y, this->targetCameraOrigin.z,
		this->upCamera.x, this->upCamera.y, this->upCamera.z);
}

void Camera::drawTarget(float radius)
{
	glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(this->targetCamera.x, this->targetCamera.y, this->targetCamera.z);
		glutSolidSphere(radius, 10, 10);
	glPopMatrix();
}

// =============== Setter  =======================================================================

void Camera::setPosCamera(glm::vec3 posCamera)
{
	this->posCamera = posCamera;
}

void Camera::setTargetCamera(glm::vec3 targetCamera)
{
	this->targetCamera = targetCamera;
}

void Camera::setUpCamera(glm::vec3 upCamera)
{
	this->upCamera = upCamera;
}


// =============== Getter  =======================================================================

glm::vec3 Camera::getPosCamera()
{
	return this->posCamera;
}

glm::vec3 Camera::getTargetCamera()
{
	return this->targetCamera;
}

glm::vec3 Camera::getUpCamera()
{
	return this->upCamera;
}

glm::mat4x4 Camera::getMatrixModelView()
{
	return this->matrixModelView;
}

glm::mat4x4 Camera::getMatrixProjection()
{
	return this->matrixProjection;
}


// =============== Actions on Camera =============================================================

void Camera::rotation_camera(float x, float y, float xOri, float yOri)
{
	// Get the delta angle
	float deltaAngle_x = (float)(x - xOri) * this->speedCamera;
	float deltaAngle_y = (float)(y - yOri) * this->speedCamera;

	this->angleHorizontal += deltaAngle_x;
	this->angleVertical += deltaAngle_y;

	glm::vec3 vectorModified;
	vectorModified.x = this->lengthPositionToTarget * cosf(angleHorizontal) * cosf(angleVertical);
	vectorModified.z = this->lengthPositionToTarget * sinf(angleHorizontal) * cosf(angleVertical);
	vectorModified.y = this->lengthPositionToTarget * sinf(angleVertical);

	this->targetCamera = posCamera + vectorModified;
}

void Camera::translation_camera(float x, float y, float xOri, float yOri)
{
	// Translation
	float deltaPos_x = (x - xOri) * speedCamera *4.0f;
	float deltaPos_y = (y - yOri) * speedCamera *4.0f;

	this->targetCamera += deltaPos_x * this->vectorNormalRight;
	this->posCamera += deltaPos_x * this->vectorNormalRight;
	this->targetCamera += deltaPos_y * this->vectorNormalTop;
	this->posCamera += deltaPos_y * this->vectorNormalTop;
}

void Camera::zoom_camera(bool ZoomIn)
{
	glm::vec3 vecPosCamToTarget = this->targetCamera - this->posCamera;
	vecPosCamToTarget = glm::normalize(vecPosCamToTarget);
	//printf("\nVecPosCamToTarget (%f, %f, %f)", vecPosCamToTarget.x, vecPosCamToTarget.y, vecPosCamToTarget.z);
	float valueMultiplySpeed = 250.0f;
	if (ZoomIn) {
		//printf("\nCam before (%f, %f, %f)", this->posCamera.x, this->posCamera.y, this->posCamera.z );
		this->targetCamera += vecPosCamToTarget * speedCamera * valueMultiplySpeed;
		this->posCamera += vecPosCamToTarget * speedCamera * valueMultiplySpeed;
		//printf("\nCam after (%f, %f, %f)", this->posCamera.x, this->posCamera.y, this->posCamera.z);
	}
	else {
		this->targetCamera -= vecPosCamToTarget * speedCamera * valueMultiplySpeed;
		this->posCamera -= vecPosCamToTarget * speedCamera * valueMultiplySpeed;
	}
	
}

void Camera::updateNormalVectors()
{
	float angleVerticalPlusHalfPi = angleVertical + M_PI / 2.0f;
	float angleHorizontalModified = this->angleHorizontal;
	if (angleVerticalPlusHalfPi > M_PI) {
		angleVerticalPlusHalfPi = 2 * M_PI - angleVerticalPlusHalfPi;
		angleHorizontalModified += M_PI;
	}

	this->vectorNormalRight.x = this->lengthPositionToTarget * cosf(angleHorizontal - M_PI / 2.0f);
	this->vectorNormalRight.z = this->lengthPositionToTarget * sinf(angleHorizontal - M_PI / 2.0f);
	this->vectorNormalRight = glm::normalize(this->vectorNormalRight);

	
	this->vectorNormalTop.x = this->lengthPositionToTarget * cosf(angleHorizontalModified) * cosf(angleVerticalPlusHalfPi);
	this->vectorNormalTop.z = this->lengthPositionToTarget * sinf(angleHorizontalModified) * cosf(angleVerticalPlusHalfPi);
	this->vectorNormalTop.y = this->lengthPositionToTarget * sinf(angleVerticalPlusHalfPi);
	this->vectorNormalTop = glm::normalize(this->vectorNormalTop);
}

// ======================================= Other functions =====================================

void Camera::printParameters()
{
	printf("\n---------------------");
	printf("\nParameters of Camera : ");
	printf("\n - Position (%f, %f, %f)", posCamera.x, posCamera.y, posCamera.z);
	printf("\n - target (%f, %f, %f)", targetCamera.x, targetCamera.y, targetCamera.z);
	printf("\n - Up (%f, %f, %f)", upCamera.x, upCamera.y, upCamera.z);
	printf("\n---------------------");
}



// =================================================================================================
// ====================== PRIVATE FUNCTIONS ========================================================



float Camera::calculAngleHorizontal(glm::vec3 myVector)
{
	return (float)(atan2(myVector.z, myVector.x));
}

float Camera::calculAngleVertical(glm::vec3 myVector)
{
	return asinf(myVector.y / glm::length(myVector));
}