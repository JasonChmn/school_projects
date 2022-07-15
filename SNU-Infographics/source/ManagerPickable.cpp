#include "ManagerPickable.h"

ManagerPickable::ManagerPickable()
{
	this->numberElementsInList = 0;
	this->projectionMatrixUpdated = false;
	this->indexObjectPicked = -1;
}


ManagerPickable::~ManagerPickable()
{
}


// =========================================== ADD or Remove from the list ==========================================

int ManagerPickable::addObjectPickable(float radiusCollisionBox, glm::vec3 * _posObject)
{
	pickableObject newObject = { NULL, -1 };
	int index = isListed(_posObject, radiusCollisionBox);
	int indexNewElement = -1;
	// If object not in list, we add it
	if (index == -1) {
		newObject._posObject = _posObject;
		newObject.radiusCollisionBox = radiusCollisionBox;
		this->listPickableObjects.push_back(newObject);
		this->numberElementsInList++;
		indexNewElement = this->numberElementsInList - 1;
	}
	return indexNewElement;
}

// !! Indices of every points will change if we remove an object which is not at the end. Do not use for the moment.
bool ManagerPickable::removeObjectPickable(float radiusCollisionBox, glm::vec3 * _posObject)
{
	bool successErasing = false;
	int index = isListed(_posObject, radiusCollisionBox);
	if (index != -1) {
		this->listPickableObjects.erase(this->listPickableObjects.begin() + index);
		this->numberElementsInList--;
	}
	return successErasing;
}




void ManagerPickable::updateNearAndFar(float lengthNear, float lengthFar)
{
	this->lengthCamToNear = lengthNear;
	this->lengthCamToFar = lengthFar;
}


void ManagerPickable::updateInverseProjViewMatrix()
{
	if (projectionMatrixUpdated == false) {
		// update projection matrix
		float m[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
		glGetFloatv(GL_PROJECTION_MATRIX, m);
		for (int col = 0; col < 4; col++) {
			for (int row = 0; row < 4; row++) {
				this->projectionMatrix[col][row] = m[col * 4 + row];
			}
		}
	}
	// update modelViewMatrix
	float m[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glm::mat4x4 modelViewMatrix;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			modelViewMatrix[col][row] = m[col * 4 + row];
		}
	}
	// calculate the inverse of (Proj*view)
	this->inverse_projectionViewMatrix = glm::inverse(this->projectionMatrix*modelViewMatrix);
}


int ManagerPickable::tryToPickObject(int xWindow, int yWindow)
{
	glm::vec3 posNearPlan, posFarPlan;

	find_2Dto3Dcoords_on_near_far_plans(xWindow, yWindow, &posNearPlan, &posFarPlan);
	//printf("\nRAY - ");
	checkRayCast(posNearPlan, posFarPlan, &this->tValueOfObjectPicked);
	//printf("OK %i", indexObjectPicked);
	return indexObjectPicked;
}

int ManagerPickable::getIndexObjectPicked()
{
	return this->indexObjectPicked;
}


bool ManagerPickable::moveObjectPicked(int xWindow, int yWindow)
{
	if (indexObjectPicked != -1) {
		glm::vec3 posNearPlan, posFarPlan;
		glm::vec3 vectorNearFar;
		glm::vec3 newPosObject;
		float ratio_thales = 0.0f;

		find_2Dto3Dcoords_on_near_far_plans(xWindow, yWindow, &posNearPlan, &posFarPlan);
		vectorNearFar = posFarPlan - posNearPlan;
		ratio_thales = this->tValueOfObjectPicked / (this->lengthCamToFar - this->lengthCamToNear);

		newPosObject = posNearPlan + vectorNearFar * ratio_thales;
		moveObjectToPosition(indexObjectPicked, newPosObject);
	}
	return (indexObjectPicked != -1);
}


void ManagerPickable::releaseobject()
{
	this->indexObjectPicked = -1;
}

void ManagerPickable::drawLinesToOriginObjectPicked()
{	
	if (this->indexObjectPicked != -1) {
		//printf("\nIndex object : %i", this->indexObjectPicked);
		glm::vec3* positionObjectPicked = this->getPositionObjectPicked();
		glColor3f(0.7f, 0.7f, 0.7f);
		glBegin(GL_LINES);
			glVertex3f(positionObjectPicked->x, positionObjectPicked->y, positionObjectPicked->z);
			glVertex3f(positionObjectPicked->x, 0.0f, positionObjectPicked->z);	// XYZ -> XZ
			glVertex3f(positionObjectPicked->x, positionObjectPicked->y, positionObjectPicked->z);
			glVertex3f(positionObjectPicked->x, positionObjectPicked->y, 0.0f);	// XYZ -> XY
			glVertex3f(positionObjectPicked->x, positionObjectPicked->y, positionObjectPicked->z);
			glVertex3f(0.0f, positionObjectPicked->y, positionObjectPicked->z);	// XYZ -> YZ

			glVertex3f(positionObjectPicked->x, 0.0f, positionObjectPicked->z);
			glVertex3f(positionObjectPicked->x, 0.0f, 0.0f);	// XZ -> X
			glVertex3f(positionObjectPicked->x, 0.0f, positionObjectPicked->z);
			glVertex3f(0.0f, 0.0f, positionObjectPicked->z);	// XZ -> Z

			glVertex3f(positionObjectPicked->x, positionObjectPicked->y, 0.0f);
			glVertex3f(positionObjectPicked->x, 0.0f, 0.0f);	// XY -> x
			glVertex3f(positionObjectPicked->x, positionObjectPicked->y, 0.0f);
			glVertex3f(0.0f, positionObjectPicked->y, 0.0f);	// XY -> y

			glVertex3f(0.0f, positionObjectPicked->y, positionObjectPicked->z);
			glVertex3f(0.0f, positionObjectPicked->y, 0.0f);	// YZ -> Y
			glVertex3f(0.0f, positionObjectPicked->y, positionObjectPicked->z);
			glVertex3f(0.0f, 0.0f, positionObjectPicked->z);	// YZ -> Z

			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(positionObjectPicked->x, 0.0f, 0.0f);	// 0 -> X
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, positionObjectPicked->y, 0.0f);	// 0 -> Y
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, positionObjectPicked->z);	// 0 -> Z
		glEnd();
		glLineStipple(1, 0XFFFF);
	}
}




//==============================================================================================================================
//====================================== PRIVATE FUNCTIONS =====================================================================

// =================================== CHECK IF AN OBJECT IS IN LIST ================================================
int ManagerPickable::isListed(glm::vec3 * _vector_tested, float radiusCollisionBox)
{
	int index = 0;
	bool found = false;
	pickableObject objectTested = { _vector_tested, radiusCollisionBox };
	while (index < this->numberElementsInList && !found) {
		if (ObjectEquals(this->listPickableObjects[index], objectTested)) {
			found = true;
		}
		index++;
	}
	if (!found) {
		index = 0;
	}
	return index - 1;
}


bool ManagerPickable::ObjectEquals(pickableObject obj1, pickableObject obj2)
{
	bool equals = false;
	if (obj1.radiusCollisionBox == obj2.radiusCollisionBox) {
		if (obj1._posObject == obj2._posObject) {
			equals = true;
		}
	}
	return equals;
}


void ManagerPickable::find_2Dto3Dcoords_on_near_far_plans(	int xWindow, int yWindow, 
															glm::vec3 * output_posNearPlan, glm::vec3 * output_posFarPlan)
{
	// variables
	GLdouble pos3D_near_x, pos3D_near_y, pos3D_near_z;
	GLdouble pos3D_far_x, pos3D_far_y, pos3D_far_z;
	GLint viewportParameters[4] = { 0,0,0,0 };
	glGetIntegerv(GL_VIEWPORT, viewportParameters);

	// 1 - normalize value of x and y, in NDC (between [-1,1])
	float x_ndc = 2.0f * ((float)(xWindow) / (float)viewportParameters[2]) - 1;	// 2*(Xwindow - windowX_coordinates / width_view) -1
	float y_ndc = -2.0f * ((float)(yWindow) / (float)viewportParameters[3]) + 1;	// Inverse for the Y axis on screen
																					//printf("\n --- x_ndc(%f), y_ndc(%f)", x_ndc, y_ndc);
	glm::vec4 pos_ndc_near(x_ndc, y_ndc, 0.0f, 1.0f);
	glm::vec4 pos_ndc_far(x_ndc, y_ndc, 1.0f, 1.0f);

	// 2 - Multiply by the inverse matrix, to get 3D coordinates
	glm::vec4 pos_3D_near = inverse_projectionViewMatrix * pos_ndc_near;
	glm::vec4 pos_3D_far = inverse_projectionViewMatrix * pos_ndc_far;
	//printf("\n3D : Position after multi : x(%f) y(%f) z(%f) w(%f)", pos_3D[0], pos_3D[1], pos_3D[2], pos_3D[3]);
	// Undo the perspective division : divide by W
	if (pos_3D_near[3] != 0.0) {
		//printf("\nPos 3D [3] = %f", pos_3D_near[3]);
		pos_3D_near[0] /= pos_3D_near[3];
		pos_3D_near[1] /= pos_3D_near[3];
		pos_3D_near[2] /= pos_3D_near[3];
	}
	else {
		printf("\nError in find_2D_coords_near_far_plans : near !!!!");
		Sleep(3000);
		exit(0);
	}
	if (pos_3D_far[3] != 0.0) {
		//printf("\nPos 3D [3] = %f", pos_3D_far[3]);
		pos_3D_far[0] /= pos_3D_far[3];
		pos_3D_far[1] /= pos_3D_far[3];
		pos_3D_far[2] /= pos_3D_far[3];
	}
	else {
		printf("\nError in find_2D_coords_near_far_plans : far !!!!");
		Sleep(3000);
		exit(0);
	}
	pos3D_near_x = pos_3D_near.x; pos3D_near_y = pos_3D_near.y; pos3D_near_z = pos_3D_near.z;
	pos3D_far_x = pos_3D_far.x; pos3D_far_y = pos_3D_far.y; pos3D_far_z = pos_3D_far.z;

	// 3 - Put values in output variables
	output_posNearPlan->x = pos3D_near_x; output_posNearPlan->y = pos3D_near_y; output_posNearPlan->z = pos3D_near_z;
	output_posFarPlan->x = pos3D_far_x; output_posFarPlan->y = pos3D_far_y; output_posFarPlan->z = pos3D_far_z;
}


// =========================================== RAY CAST =============================================================

void ManagerPickable::checkRayCast(glm::vec3 posNearPlan, glm::vec3 posFarPlan, float * _outputTValue)
{
	glm::vec3 vectorDirection = { posFarPlan.x - posNearPlan.x , posFarPlan.y - posNearPlan.y , posFarPlan.z - posNearPlan.z };
	glm::vec3 vectorDirectionNormalized = glm::normalize(vectorDirection);
	float lengthVectorDirection = glm::length(vectorDirection);
	int indexFound = -1;
	int indexActual = 0;
	glm::vec3 * posObjectActual = NULL;
	glm::vec3 nearestPoint = { 0,0,0 };
	float tValueFound = 0;	// Value of t on parametric equation ( ray characterized by vectorDirection )
	float minTValueFound = 10000000;


	//printf("\nvectorDirection (%f, %f, %f)", vectorDirection.x, vectorDirection.y, vectorDirection.z);

	glm::vec3 vecNearToObject;
	glm::vec3 minVecNearRayToObject;


	while (indexActual < this->numberElementsInList) {
		posObjectActual = listPickableObjects[indexActual]._posObject;
		//printf("\n\nObject n(%i) at position (%f, %f, %f)", indexActual, posObjectActual->x, posObjectActual->y, posObjectActual->z);

		// actualize vecNearToObject
		vecNearToObject = *posObjectActual - posNearPlan;
		tValueFound = glm::dot(vecNearToObject, vectorDirectionNormalized);

		// if tvalue inferior to a previous minTValue found with collision, then this new object will be in front of the previous.
		// otherwise, it will be behind, so we don't take it in account.
		if (tValueFound < minTValueFound) {
			// if tValueFound < 0 -> point behind the near plan, so we don't take it in account.
			// if tValueFound > lengthVectorDirection -> point after the far plan, so we don't take it in account
			if (tValueFound >= 0 && tValueFound <= lengthVectorDirection) {
				nearestPoint = posNearPlan + vectorDirectionNormalized*tValueFound;
				//printf("\nNearest point on ray (%f,%f,%f)", nearestPoint.x, nearestPoint.y, nearestPoint.z);
				// We check if the distance between the nearestPoint on ray and the object is inferior to the radius of his collision box.
				if (glm::distance(nearestPoint, *posObjectActual) <= listPickableObjects[indexActual].radiusCollisionBox) {
					//printf("\nCollision !");
					minTValueFound = tValueFound;
					indexFound = indexActual;
					minVecNearRayToObject = nearestPoint;
				}
			}
		}
		indexActual++;
	}

	this->indexObjectPicked = indexFound;
	*_outputTValue = minTValueFound;
}

void ManagerPickable::moveObjectToPosition(int index, glm::vec3 newPosition)
{
	glm::vec3* positionObjectPicked;
	if (index <= this->numberElementsInList - 1) {
		positionObjectPicked = this->getPositionObjectPicked();
		positionObjectPicked->x = newPosition.x;
		positionObjectPicked->y = newPosition.y;
		positionObjectPicked->z = newPosition.z;
	}
}

glm::vec3 * ManagerPickable::getPositionObjectPicked()
{
	glm::vec3* pointerPosition = NULL;
	if (this->indexObjectPicked != -1) {
		pointerPosition = this->listPickableObjects[this->indexObjectPicked]._posObject;
	}
	return pointerPosition;
}
