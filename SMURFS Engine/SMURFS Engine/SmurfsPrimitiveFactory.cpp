/**************************************************************************************************
* Title: SmurfsPrimitiveFactor.cpp
* Authors: Gael Huber
* Description: Creates primitives 
**************************************************************************************************/
#include "SmurfsPrimitiveFactory.h"

/**************************************************************************************************
* Constructor
**************************************************************************************************/
PrimitiveFactory::PrimitiveFactory(void) {
	// Do nothing
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
PrimitiveFactory::~PrimitiveFactory(void) {
	// Do nothing
}

/**************************************************************************************************
* Returns a spherical mesh
*
* float radius	Radius of the sphere
* int segments		Number of segments into which the sphere will be split
**************************************************************************************************/
Mesh* PrimitiveFactory::createSphere(float radius, int segments) {
	// Define the standard segment radius
	float segmentRadius = PI / 2.0f / (segments + 1);

	// Number of separators for any given disc
	int numberOfSeparators = 4 * segments + 4;

	// Calculate the number of points that will appear in the sphere
	unsigned int numPoints = (segments * 2) * numberOfSeparators;

	// Define and initialize an array of points
	Vector3* points = new Vector3[numPoints];
	for(int i = 0; i < numPoints; i++)
		points[i] = Vector3();

	// Iterate through and define all points that compose the circle
	for(int segs = -segments; segs <= segments; segs++) {
		// Define the local radius of a particular disc
		float r = radius * cos(segmentRadius * segs);
		float y = radius * sin(segmentRadius * segs);

		// Create the local disc
		for(int seps = 0; seps < numberOfSeparators; seps++) {

		}
	}






	// Define vertices and colors
	float* vertices = new float[numPoints * 3];

	return new Mesh();
}