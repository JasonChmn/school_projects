#pragma once

#include "../libraries\glew\glew.h"
#include "../libraries\freeglut\freeglut.h"
#include "../libraries\glm\glm\matrix.hpp"
#include <vector>

class ManagerPickable
{
public:
	ManagerPickable();
	~ManagerPickable();

	// Add an object to the list, return ID of object if added, -1 if not added.
	int addObjectPickable(float radiusCollisionBox, glm::vec3 * _posObject);
	// Remove an object from the list, return false if we did not find the object.
	// !! Indices of every points will change if we remove an object which is not at the end. Do not use for the moment.
	bool removeObjectPickable(float radiusCollisionBox, glm::vec3 * _posObject);

	// Update Near and Far
	void updateNearAndFar(float lengthNear, float lengthFar);

	// Update inverse of projection*modView matrix
	void updateInverseProjViewMatrix();

	// Pick an object (to call when we click)
	int tryToPickObject(int xWindow, int yWindow);	// return index of object picked
	// Get index of object picked
	int getIndexObjectPicked();
	// Move object picked
	bool moveObjectPicked( int xWindow, int yWindow );
	// Release an object (to call when unclick)
	void releaseobject();

	// DRAW LINES WHEN WE MOVE OBJECT TO LOCATE OBJECT COMPARE TO AXIS
	void drawLinesToOriginObjectPicked();

private:
	int indexObjectPicked;	// -1 if nothing is picked
	float tValueOfObjectPicked = -1.0f;	// Value --> Vector NearToObject = tValue * normalize(Vector NearToFar)

	// structure of my objects
	struct pickableObject {
		glm::vec3 * _posObject;
		float radiusCollisionBox;
	};

	// Inverse of (projection*modelViewMatrix)
	glm::mat4x4 inverse_projectionViewMatrix;
	glm::mat4x4 projectionMatrix;
	bool projectionMatrixUpdated;

	// Length camera to Near and Far, size view
	float lengthCamToNear, lengthCamToFar;

	// Check if two objects are equals
	bool ObjectEquals(pickableObject obj1, pickableObject obj2);

	// Check if the adresse of vector correspond to a vector in our list
	int isListed(glm::vec3 * _vector_tested, float radiusCollisionBox);

	int numberElementsInList;
	std::vector<pickableObject> listPickableObjects;

	// convert pos on screen 2D -> pos on near and far plan in 3D space
	void find_2Dto3Dcoords_on_near_far_plans(	int xWindow, int yWindow,
												glm::vec3* output_posNearPlan, glm::vec3* output_posFarPlan);

	// Cast a ray, and check if we touch an object, change variable at _outputID, -1 if we find nothing.
	// Change the _outputTValue, to know the distance between NearPlane->NearestPointOnRay : NearestPointOnRay = *_outputTValue * (PosFarPlan - posNearPlan) + posNearPlan
	void checkRayCast(glm::vec3 posNearPlan, glm::vec3 posFarPlan, float * _outputTValue);

	// Move object at index to a position
	void moveObjectToPosition(int index, glm::vec3 newPosition);

	// Get position of object picked
	glm::vec3* getPositionObjectPicked();
};