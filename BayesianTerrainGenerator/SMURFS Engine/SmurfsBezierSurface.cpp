/**************************************************************************************************
* Title: SmurfsBezierSurface.cpp
* Authors: Gael Huber
* Description: Describes a bezier surface and the algorithms for generating points at a given
*	resolution.
**************************************************************************************************/
#include "SmurfsBezierSurface.h"

/**************************************************************************************************
* Default constructor 
**************************************************************************************************/
BezierSurface::BezierSurface(void) {
	vertexMap = 0;
}

/**************************************************************************************************
* Constructor that defines position, dimensions, and resolution
**************************************************************************************************/
BezierSurface::BezierSurface(Vector3 c, float w, float h, unsigned int r) {
	// Construct the body for the vertex map
	vertexMap = (Vector3**) calloc(r, sizeof(Vector3*));
	for(unsigned int i = 0; i < r; i++) {
		vertexMap[i] = (Vector3*) calloc(r, sizeof(Vector3));
		vert.push_back(std::vector<Vector3>());
	}

	// Initialize matrix
	for(unsigned int i = 0; i < r; i++) {
		for(unsigned int j = 0; j < r; j++) {
			vertexMap[i][j] = Vector3();
		}
	}

	// Save key components
	center = c;
	width = w;
	height = h;
	resolution = r;

	// Load in the constant matrix B
	B = MatrixReader::getSingletonPtr()->loadMatrix("B.mat");
	BT = B.getTranspose();

	// Initialize matrix P
	P = Matrix(4, 4);
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
BezierSurface::~BezierSurface(void) {
	// Free all memory used for the matrix
	for(unsigned int i = 0; i < resolution; i++) {
		
		free(vertexMap[i]);
	}
	free(vertexMap);
}

/**************************************************************************************************
* Builds the surface given a matrix P
*											[v^3]
* P(u, v) = [u^3 u^2 u 1] x [B x P x B^T] x	[v^2]
*											[ v ]
*											[ 1 ]
**************************************************************************************************/
void BezierSurface::buildSurface(void) {
	// x and y values for the vertexMap
	float x = -width / 2.0f;
	float z = -height / 2.0f;
	float xStep = (float) width / (float) resolution;
	float zStep = (float) height / (float) resolution;
	float u = 0.0f;
	float v = 0.0f;

	// Build the vertex map
	for(unsigned int i = 0; i < resolution; i++) {
		// Calculate the current u
		u = (float) i / (float) resolution;

		// Create a u matrix
		Matrix uMat = Matrix(1, 4);
		uMat.matrix[0][0] = u * u * u;
		uMat.matrix[0][1] = u * u;
		uMat.matrix[0][2] = u;
		uMat.matrix[0][3] = 1.0f;

		for(unsigned int j = 0; j < resolution; j++) {
			// Calculate the current v
			v = (float) j / (float) resolution;

			// Create a v matrix
			Matrix vMat = Matrix(4, 1);
			vMat.matrix[0][0] = v * v * v;
			vMat.matrix[1][0] = v * v;
			vMat.matrix[2][0] = v;
			vMat.matrix[3][0] = 1.0f;

			// Calculate the value for the vertex map
			Matrix bpbt = (B * P) * BT;	// The matrix representative of BxPxB^T
			Matrix pUV = (uMat * bpbt) * vMat;	// Matrix holding the y value for the particular point
			vertexMap[i][j] = Vector3(x, pUV.matrix[0][0], z);	// Assign the values to the vertex map

			vert[i].push_back(vertexMap[i][j]);

			// Increment the z position
			z += zStep;
		}

		// Increment the x position and reset z
		x += xStep;
		z = -height / 2.0f;
	}
}