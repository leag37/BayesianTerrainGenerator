/**************************************************************************************************
* Title: SmurfsRenderObject.h
* Authors: Gael Huber
* Description: Wrapper class for a render object
**************************************************************************************************/
#ifndef __SMURFSRENDEROBJECT_H__
#define __SMURFSRENDEROBJECT_H__

//#include <GL/GL.h>
//#include <GL/GLU.h>

#include "SmurfsVector3.h"

class RenderObject {
public:
	RenderObject(void);	// Constructor
	~RenderObject(void);	// Destructor

	unsigned int data;		// Object's draw list
	float screenX, screenY;	// On-screen coordinates
	float angle;			// Angle of rotation
	int width, height;		// Dimensions
	float halfWidth, halfHeight;	// Half dimensions
	int resourceID;		// The resource ID for this object
	int objectID;		// Unique ID for this object

	//GLfloat rotateAngle;	// Angle for object's rotation
	Vector3 rotationAxis;	// Axis for object's rotation
};

#endif