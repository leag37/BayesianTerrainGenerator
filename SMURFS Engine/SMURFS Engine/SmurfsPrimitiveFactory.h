/**************************************************************************************************
* Title: SmurfsPrimitiveFactor.h
* Authors: Gael Huber
* Description: Creates primitives 
**************************************************************************************************/
#ifndef __SMURFSPRIMITIVEFACTORY_H__
#define __SMURFSPRIMITIVEFACTORY_H__

#include "SmurfsMesh.h"
#include <math.h>

#define PI 3.14159265f

class PrimitiveFactory {
public:
	PrimitiveFactory(void);		// Constructor
	~PrimitiveFactory(void);	// Destructor

	static Mesh* createSphere(float radius, int segments);	// Return a spherical mesh
};

#endif