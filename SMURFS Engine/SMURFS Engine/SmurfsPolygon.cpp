/**************************************************************************************************
* Title: SmurfsPolygon.cpp
* Authors: Gael Huber
* Description: Polygon used for collision detection as well as planar properties. Assumes points 
*	are laid out in a clockwise fashion.
**************************************************************************************************/
#include "SmurfsPolygon.h"

float SmurfPolygon::maxSlope = 0.0f;
float SmurfPolygon::maxHeight = 0.0f;
float SmurfPolygon::minHeight = 0.0f;

/**************************************************************************************************
* Default constructor
**************************************************************************************************/
SmurfPolygon::SmurfPolygon(void) {
	points[0] = Vector3();
	points[1] = Vector3();
	points[2] = Vector3();

	normal = Vector3();
	slope = 0.0f;
	type = NONE;
}

/**************************************************************************************************
* Constructor
**************************************************************************************************/
SmurfPolygon::SmurfPolygon(Vector3 points[3]) {
	// Populate the points of the polygon
	this->points[0] = points[0];
	this->points[1] = points[1];
	this->points[2] = points[2];

	// Calculate the normal of the polygon
	calculateNormal();

	// Calculate the slope of the polygon
	calculateSlope();

	// Calculate the center
	calculateCenter();

	type = NONE;
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
SmurfPolygon::~SmurfPolygon(void) {
	delete[] points;
}

/**************************************************************************************************
* Calculate the normal of the polygon
**************************************************************************************************/
void SmurfPolygon::calculateNormal(void) {
	// U = P1 - P0
	Vector3 u = points[1] - points[0];

	// V = P2 - P0
	Vector3 v = points[2] - points[0];
	
	// Normalize u and v
	u.normalize();
	v.normalize();

	// Normal
	normal = u.cross(v);
	normal.normalize();
}

/************************************************************************************************
* Calculate the slope of the polygon
**************************************************************************************************/
void SmurfPolygon::calculateSlope(void) {
	// Find the horizontal component
	Vector3 u = points[1] - points[0];
	u = Vector3(abs(u.x), abs(u.y), abs(u.z));
	Vector3 v = points[2] - points[0];
	v = Vector3(abs(v.x), abs(v.y), abs(v.z));

	float uxz = sqrt(u.x * u.x + u.z * u.z);
	float vxz = sqrt(v.x * v.x + v.z * v.z);

	// Calculate the slope by averaging the slopes of each horizontal component
	slope = uxz / u.y + vxz / v.y;
	slope /= 2.0f;
	slope = 1.0f / slope;

	// Find the horizontal component
	//slope = normal.y / xz;

	// Track maximum slope of the terrain to act as a real-time modifier during type generation
	if(slope > maxSlope)
		maxSlope = slope;
}

/************************************************************************************************
* Calculate the center of the polygon
**************************************************************************************************/
void SmurfPolygon::calculateCenter(void) {
	// The centroid is the average of the coordinates of each vertex
	center = (points[0] + points[1] + points[2]) / 3.0f;

	// Track maximum and minimum height of the terrain to act as a real-time modifier during type generation
	if(center.y > maxHeight)
		maxHeight = center.y;
	else if(center.y < minHeight)
		minHeight = center.y;
}

/**************************************************************************************************
* Getters and setters
**************************************************************************************************/
Vector3* SmurfPolygon::getPoints(void) { return points; }
Vector3 SmurfPolygon::getCenter(void) { return center; }
Vector3 SmurfPolygon::getColor(void) { return color; }
float SmurfPolygon::getSlope(void) { return slope; }
TERRAIN_TYPE SmurfPolygon::getType(void) { return type; }
void SmurfPolygon::setType(TERRAIN_TYPE value) { type = value; }