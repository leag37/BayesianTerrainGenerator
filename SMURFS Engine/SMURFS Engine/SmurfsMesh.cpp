/**************************************************************************************************
* Title: SmurfsRenderObject.cpp
* Authors: Gael Huber
* Description: Wrapper class for a render object
**************************************************************************************************/
#include "SmurfsMesh.h"
#include "OpenGL/glew-1.5.8/include/GL/glew.h"
#include "OpenGL/glew-1.5.8/include/GL/wglew.h"
#include <GL/GL.h>
#include <GL/GLU.h>

// Constructor
Mesh::Mesh(void) : screenCoords(Vector3(0,0,0)), dimensions(Vector3(0.5f,0.5f,0.5f)),
	halfDimensions(Vector3(0.25f,0.25f,0.25f)) {
	// Do nothing
}

// Constructor
Mesh::Mesh(RenderMode mode) : screenCoords(Vector3(0,0,0)), dimensions(Vector3(0.5f,0.5f,0.5f)),
	halfDimensions(Vector3(0.25f,0.25f,0.25f)) {
	this->mode = mode;

	modelMatrix = glm::mat4(1.0f);	// Identity matrix for the model

	/*float* vertices = new float[18];
	vertexCount = 18;
	vertices[0] = -0.5; vertices[1] = -0.5; vertices[2] = 0.0; // Bottom left corner  
	vertices[3] = -0.5; vertices[4] = 0.5; vertices[5] = 0.0; // Top left corner  
	vertices[6] = 0.5; vertices[7] = 0.5; vertices[8] = 0.0; // Top Right corner  
	vertices[9] = 0.5; vertices[10] = -0.5; vertices[11] = 0.0; // Bottom right corner  
	vertices[12] = -0.5; vertices[13] = -0.5; vertices[14] = 0.0; // Bottom left corner  
	vertices[15] = 0.5; vertices[16] = 0.5; vertices[17] = 0.0; // Top Right corner  */

	/*// Vertices in the polygon
	float* vertices;

	// Set to draw a pyramid
	if(mode == S_TRIANGLES) {
		// Declare the number of vertices
		//vertices = new Vector3[18];
		vertices = new float[54];
		vertexCount = 54;

		vertices[0] = 0.0f; vertices[1] = 0.25f; vertices[2] = 0.0f;			// Top Of Triangle (Front)
		vertices[3] = -0.25f; vertices[4] = -0.25f; vertices[5] = 0.25f;		// Left Of Triangle (Front)
		vertices[6] = 0.25f; vertices[7] = -0.25f; vertices[8] = 0.25f;			// Right Of Triangle (Front)

		vertices[9] = 0.0f; vertices[10] = 0.25f; vertices[11] = 0.0f;			// Top Of Triangle (Right)
		vertices[12] = 0.25f;vertices[13] = -0.25f; vertices[14] = 0.25f;		// Left Of Triangle (Right)
		vertices[15] = 0.25f; vertices[16] = -0.25f; vertices[17] = -0.25f;		// Right Of Triangle (Right)

		vertices[18] = 0.0f; vertices[19] = 0.25f; vertices[20] = 0.0f;			// Top Of Triangle (Back)
		vertices[21] = 0.25f; vertices[22] = -0.25f; vertices[23] = -0.25f;		// Left Of Triangle (Back)
		vertices[24] = -0.25f; vertices[25] = -0.25f; vertices[26] = -0.25f;	// Right Of Triangle (Back)

		vertices[27] =  0.0f; vertices[28] = 0.25f; vertices[29] = 0.0f;		// Top Of Triangle (Left)
		vertices[30] = -0.25f; vertices[31] = -0.25f; vertices[32] = -0.25f;	// Left Of Triangle (Left)
		vertices[33] = -0.25f; vertices[34] = -0.25f; vertices[35] = 0.25f;		// Right Of Triangle (Left)

		vertices[36] = -0.25; vertices[37] = -0.25f; vertices[38] = -0.25f;		// Top Of Triangle (Bottom)
		vertices[39] = 0.25f; vertices[40] = -0.25f; vertices[41] = -0.25f;		// Left Of Triangle (Bottom)
		vertices[42] = 0.25f; vertices[43] = -0.25f; vertices[44] = 0.25f;		// Right Of Triangle (Bottom)

		vertices[45] = 0.25f; vertices[46] = -0.25f; vertices[47] = 0.25f;		// Top Of Triangle (Bottom)
		vertices[48] = -0.25f; vertices[49] = -0.25f; vertices[50] = 0.25f;		// Left Of Triangle (Bottom)
		vertices[51] = -0.25f; vertices[52] = -0.25f; vertices[53] = -0.25f;	// Right Of Triangle (Bottom)
	} else if(mode == S_QUAD) {
		vertices = new float[72];
		vertexCount = 72;

		vertices[0] = 0.25f; vertices[1] = 0.25f; vertices[2] = -0.25f;		// Top Right Of The Quad (Top)
		vertices[3] = -0.25f; vertices[4] = 0.25f; vertices[5] = -0.25f;	// Top Left Of The Quad (Top)
		vertices[6] = -0.25f; vertices[7] = 0.25f; vertices[8] = 0.25f;			// Bottom Left Of The Quad (Top)
		vertices[9] = 0.25f; vertices[10] = 0.25f; vertices[11] = 0.25f;			// Bottom Right Of The Quad (Top)

		vertices[12] = 0.25f; vertices[13] = -0.25f; vertices[14] = 0.25f;			// Top Right Of The Quad (Bottom)
		vertices[15] = -0.25f; vertices[16] = -0.25f; vertices[17] = 0.25f;			// Top Left Of The Quad (Bottom)
		vertices[18] = -0.25f; vertices[19] = -0.25f; vertices[20] = -0.25f;			// Bottom Left Of The Quad (Bottom)
		vertices[21] = 0.25f; vertices[22] = -0.25f; vertices[23] = -0.25f;			// Bottom Right Of The Quad (Bottom)

		vertices[24] = 0.25f; vertices[25] = 0.25f; vertices[26] = 0.25f;			// Top Right Of The Quad (Front)
		vertices[27] = -0.25f; vertices[28] = 0.25f; vertices[29] = 0.25f;			// Top Left Of The Quad (Front)
		vertices[30] = -0.25f; vertices[31] = -0.25f; vertices[32] = 0.25f;			// Bottom Left Of The Quad (Front)
		vertices[33] = 0.25f; vertices[34] = -0.25f; vertices[35] = 0.25f;			// Bottom Right Of The Quad (Front)

		vertices[36] = 0.25f; vertices[37] = -0.25f; vertices[38] = -0.25f;			// Bottom Left Of The Quad (Back)
		vertices[39] = -0.25f; vertices[40] = -0.25f; vertices[41] = -0.25f;			// Bottom Right Of The Quad (Back)
		vertices[42] = -0.25f; vertices[43] = 0.25f; vertices[44] = -0.25f;			// Top Right Of The Quad (Back)
		vertices[45] = 0.25f; vertices[46] = 0.25f; vertices[47] = -0.25f;			// Top Left Of The Quad (Back)

		vertices[48] = -0.25f; vertices[49] = 0.25f; vertices[50] = 0.25f;			// Top Right Of The Quad (Left)
		vertices[51] = -0.25f; vertices[52] = 0.25f; vertices[53] = -0.25f;			// Top Left Of The Quad (Left)
		vertices[54] = -0.25f; vertices[55] = -0.25f; vertices[56] = -0.25f;			// Bottom Left Of The Quad (Left)
		vertices[57] = -0.25f; vertices[58] = -0.25f; vertices[59] = 0.25f;			// Bottom Right Of The Quad (Left)

		vertices[60] = 0.25f, vertices[61] = 0.25f,vertices[62] = -0.25f;			// Top Right Of The Quad (Right)
		vertices[63] = 0.25f, vertices[64] = 0.25f, vertices[65] = 0.25f;			// Top Left Of The Quad (Right)
		vertices[66] = 0.25f,vertices[67] = -0.25f, vertices[68] = 0.25f;			// Bottom Left Of The Quad (Right)
		vertices[69] = 0.25f,vertices[70] = -0.25f,vertices[71] = -0.25f;			// Bottom Right Of The Quad (Right)
	}


	glGenVertexArrays(1, &vaoID[0]);	// Create our Vertex Array Object
	glBindVertexArray(vaoID[0]);	// Bind our Vertex Array Object so we can use it
	glGenBuffers(1, vboID);		// Generate our Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);	// Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);	// Set the size and data of our VBO and set it to STATIC_DRAW
	glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);		// Set up our vertex attributres pointer
	glEnableVertexAttribArray(0);	// Disable our Vertex Array Object
	glBindVertexArray(0);	// Disable our Vertex Buffer Object

	delete[] vertices;	// Delete our vertices from memory*/
}

// Destructor
Mesh::~Mesh(void) {
	// Do nothing
}

/**************************************************************************************************
* Create a mesh from a set of vertices and colors
**************************************************************************************************/
void Mesh::createMesh(float* vertices, float* colors, unsigned int count) {
	vertexCount = count;
	glGenVertexArrays(1, &vaoID[0]);	// Create our Vertex Array Object
	glBindVertexArray(vaoID[0]);	// Bind our Vertex Array Object so we can use it
	glGenBuffers(2, vboID);		// Generate our Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);	// Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), vertices, GL_STATIC_DRAW);	// Set the size and data of our VBO and set it to STATIC_DRAW
	glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);		// Set up our vertex attributres pointer
	glEnableVertexAttribArray(0);	// Disable our Vertex Array Object

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);	// Disable our Vertex Buffer Object
}