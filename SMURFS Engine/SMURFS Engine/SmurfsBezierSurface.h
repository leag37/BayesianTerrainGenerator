	/**************************************************************************************************
* Title: SmurfsBezierSurface.h
* Authors: Gael Huber
* Description: Describes a bezier surface and the algorithms for generating points at a given
*	resolution.
**************************************************************************************************/
#ifndef __SMURFSBEZIERSURFACE_H__
#define __SMURFSBEZIERSURFACE_H__

#include "SmurfsMatrix.h"
#include "SmurfsMatrixReader.h"
#include "SmurfsVector3.h"
#include "SmurfsQuaternion.h"
#include <vector>
#include <stdlib.h>

class BezierSurface {
public:
	BezierSurface(void);	// Default constructor
	BezierSurface(Vector3 c, float w, float h, unsigned int r);	//	Constructor that defines position,
		// dimensions, and resolution
	~BezierSurface(void);	// Destructor

	void buildSurface(void);	// Builds the surface given a matrix P

	Matrix P;	// Contains y values for the 16 control points for the bezier surface
	unsigned int resolution;	// Resolution of the map, how many subdivisions the surface should have
	Vector3** vertexMap;	// Grid representation of vertices
	std::vector<std::vector<Vector3>> vert; // TEMP
	Vector3 center;	// Center of the surface
	float width;	// Width of the surface
	float height;	// Height of the surface

private:
	Matrix B;	// Constant matrix B used for Bezier surfaces
	Matrix BT;	// Constant matrix BT (Transpose of B) used for Bezier surfaces
};

#endif