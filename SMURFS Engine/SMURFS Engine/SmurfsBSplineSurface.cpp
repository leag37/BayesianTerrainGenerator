/**************************************************************************************************
* Title: SmurfsBSplineSurface.cpp
* Authors: Gael Huber
* Description: Defines a B-spline surface given a set of control points.
**************************************************************************************************/
#include "SmurfsBSplineSurface.h"

/**************************************************************************************************
* Constructor
**************************************************************************************************/
BSplineSurface::BSplineSurface(void) {
	Ms = MatrixReader::getSingletonPtr()->loadMatrix("Ms.mat");
	Ms /= 6.0f;
	MsT = Ms.getTranspose();
	width = 0;
	height = 0;
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
BSplineSurface::~BSplineSurface(void) {
}

/**************************************************************************************************
* Build the surface given a set of control points, width and height of array, and the resolution at
* which to build.
*
* float** controls	2D array of index-based control points
* int w		Width of controls array
* int h		Height of controls array
* int r		Resolution at which to build the surface
**************************************************************************************************/
void BSplineSurface::build(float** controls, int w, int h, int r) {
	// Define vertex array width and height
	width = (w-3) * r;
	height = (h-3) * r;

	Matrix uMat = Matrix(1, 4);	// Matrix representing current u value
	Matrix vMat = Matrix(4, 1);	// Matrix representing current v value
	Matrix val = Matrix(4, 4);	// Value matrix

	// Adjustment values to ensure a normalized terrain is built
	float maxW = (float)(w - 3) - 1.0f/(float)r;
	float maxH = (float)(h - 3) - 1.0f/(float)r;

	// Initialize the array representing the vertices of the surface
	vertices = new Vector3*[width];
	for(unsigned int i = 0; i < width; i++) {
		vertices[i] = new Vector3[height];

		for(unsigned int j = 0; j < height; j++) {
			vertices[i][j] = Vector3();
		}
	}

	// Iterate through the control points and construct the surface
	for(int i = 2; i < w - 1; i++) {
		for(int j = 2; j < h - 1; j++) {
			// Create a 4x4 matrix from the immediately surrounding and appropriate points
			Matrix p = Matrix(4, 4);
			int x = 0;
			int z = 0;

			for(int pi = i - 2; pi <= i + 1; pi++) {
				for(int pj = j - 2; pj <= j + 1; pj++) {
					p.matrix[x][z] = controls[pi][pj];
					z++;
				}
				x++;
				z = 0;
			}
			

			// Loop through the resolution
			float res = 1.0f / (float) r;
			for(int u = 0; u < r; u++) {
				float uf = (float)u * res;
				uMat.matrix[0][0] = 1.0f;
				uMat.matrix[0][1] = uf;
				uMat.matrix[0][2] = uf*uf;
				uMat.matrix[0][3] = uf*uf*uf;

				for(int v = 0; v < r; v++) {					
					float vf = (float)v * res;
					vMat.matrix[0][0] = 1.0f;
					vMat.matrix[1][0] = vf;
					vMat.matrix[2][0] = vf*vf;
					vMat.matrix[3][0] = vf*vf*vf;

					// val = [u^T] * [Ms * P * Ms^t] * [v]
					val = ( ( ( uMat * Ms ) * p ) * MsT ) * vMat;
					vertices[(i-2)*r + u][(j-2)*r + v] = Vector3((float)(i-2) / maxW + (float)u * res / maxW, val.matrix[0][0], (float)(j-2) / maxH + (float)v * res / maxH);
				}
			}
		}
	}
}

/**************************************************************************************************
* Getters and setters
**************************************************************************************************/
unsigned int BSplineSurface::getWidth(void) { return width; }

unsigned int BSplineSurface::getHeight(void) { return height; }

Vector3** BSplineSurface::getVertices(void) { return vertices; }