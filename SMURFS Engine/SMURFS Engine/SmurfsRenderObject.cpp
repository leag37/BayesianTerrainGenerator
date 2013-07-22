/**************************************************************************************************
* Title: SmurfsRenderObject.cpp
* Authors: Gael Huber
* Description: Wrapper class for a render object
**************************************************************************************************/
#include "SmurfsRenderObject.h"

// Constructor
RenderObject::RenderObject(void) : data(0), screenX(0), screenY(0), angle(0.0f), width(0), height(0),
	halfWidth(0), halfHeight(0), resourceID(-1), objectID(-1) {
	// Do nothing
}

// Destructor
RenderObject::~RenderObject(void) {
	// Do nothing
}