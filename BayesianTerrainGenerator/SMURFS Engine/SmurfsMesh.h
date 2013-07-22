/**************************************************************************************************
* Title: SmurfsRenderObject.h
* Authors: Gael Huber
* Description: Wrapper class for a render object
**************************************************************************************************/
#ifndef __SMURFSMESH_H__
#define __SMURFSMESH_H__

#include "SmurfsVector3.h"
#include "OpenGL/glm-0.9.1.1/glm.hpp"
#include "OpenGL/glm-0.9.1.1/gtc/matrix_transform.hpp"
#include <vector>

enum RenderMode {
	S_POINTS,
	S_LINE_STRIP, S_LINE_LOOP, S_LINES, 
	S_TRIANGLE_STRIP, S_TRIANGLE_FAN, S_TRIANGLES, 
	S_QUAD_STRIP, S_QUAD
};

class Mesh {
public:
	Mesh(void);				// Constructor
	Mesh(RenderMode mode);	// Constructor taking mesh type
	~Mesh(void);			// Destructor

	void generateObjects(void);	// Generate the VBOs and VAO for the mesh
	void createMesh(float* vertices, float* colors, unsigned int count);	// Create a mesh from a set of 
		// vertices and colors

	glm::mat4 modelMatrix;	// Model matrix for the object

	unsigned int vertexCount;	// The number of vertices in this mesh
	unsigned int vboID[2];			// Vertex Buffer Object
	unsigned int vaoID[1];			// Vertex Array Object

	Vector3 screenCoords;	// On-screen coordinates
	Vector3 dimensions;		// Dimensions of the object
	Vector3 halfDimensions;	// Half dimensions for determining screen position
	RenderMode mode;		// Type of rendering to be done with the mesh

};

#endif