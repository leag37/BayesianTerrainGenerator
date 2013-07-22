/**************************************************************************************************
* Title: SmurfsPolygon.h
* Authors: Gael Huber
* Description: Polygon used for collision detection as well as planar properties. Assumes points 
*	are laid out in a clockwise fashion.
**************************************************************************************************/
#ifndef __SMURFSPOLYGON_H__
#define __SMURFSPOLYGON_H__

#include "SmurfsVector3.h"

// Property of the polygon, related to terrain
enum TERRAIN_TYPE { SNOW = 0, ROCK, ICE, GRASS, SAND, WATER, NONE };

class SmurfPolygon {
public:
	SmurfPolygon(void);		// Default constructor
	SmurfPolygon(Vector3 points[3]);	// Constructor
	~SmurfPolygon(void);		// Destructor

	// Getters and setters
	Vector3* getPoints(void);
	Vector3 getCenter(void);
	Vector3 getColor(void);
	float getSlope(void);
	TERRAIN_TYPE getType(void);
	void setType(TERRAIN_TYPE value);

	static float maxSlope;
	static float maxHeight;
	static float minHeight;

private:
	inline void calculateNormal(void);		// Calculate the normal of the polygon
	inline void calculateSlope(void);		// Calculate the slope of the polygon
	inline void calculateCenter(void);		// Calculate the center of the polygon

	Vector3 points[3];	// Series of points in the polygon
	Vector3 center;		// Center of the polygon
	Vector3 normal;		// Normal to the polygon assuming clockwise (left-handed system)
	Vector3 color;		// Color of the polygon
	float slope;		// The slope of the polygon, measures rate of incline for the polygon as a whole
	TERRAIN_TYPE type;	// The type of the polygon based on the bayesian stuff
};

#endif