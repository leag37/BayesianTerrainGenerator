/**************************************************************************************************
* Title: SmurfsBSplineSurface.h
* Authors: Gael Huber
* Description: Defines a B-spline surface given a set of control points.
**************************************************************************************************/
#ifndef __SMURFSBSPLINESURFACE_H__
#define __SMURFSBSPLINESURFACE_H__

#include "SmurfsMatrixReader.h"
#include "SmurfsVector3.h"
#include <vector>

using namespace std;

class BSplineSurface {
public:
	BSplineSurface(void);		// Constructor
	~BSplineSurface(void);		// Destructor

	void build(float** controls, int w, int h, int r);		// Build the surface given a set of control points, 
		// width and height of array, and the resolution at which to build.

	// Getters and setters
	unsigned int getWidth(void);
	unsigned int getHeight(void);
	Vector3** getVertices(void);
private:
	void initializeVertices(int w, int h);	// Properly initializes the vertices function given the 
		// width and height of the control points array
	

	float** points;		// Points of the surface
	Matrix Ms;		// Constant matrix Ms used for B-spline surfaces
	Matrix MsT;		// Transpose of Ms
	Vector3** vertices;		// A map of all vertices in the surface
	unsigned int width;		// Width of the array
	unsigned int height;	// Height of the array
};

#endif