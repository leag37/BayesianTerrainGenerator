/**************************************************************************************************
* Title: SmurfsRigidBody.h
* Author: Gael Huber
* Description: Holds a rigid body used for physics and collision systems.
**************************************************************************************************/
#ifndef __SMURFSRIGIDBODY_H__
#define __SMURFSRIGIDBODY_H__

#include "SmurfsVector3.h"
#include "SmurfsQuaternion.h"
#include <random>
#include <time.h>

enum MOVEMENT_PATTERN {
	STANDARD,	// Just use regular velocity
	HELIX,		// Move in a helix
	PARABOLA,	// Move in a parabolic fashion
	CIRCLE		// Move in a circular motion
};

class RigidBody {
public:
	RigidBody(void);	// Constructor
	RigidBody(Vector3 axes, Vector3 halfWidths);	// Constructor defining dimensions
	~RigidBody(void);	// Destructor

	void move(void);	// Move the rigid body according to it's particularly movement pattern

	Vector3 position;	// Position of rigid body
	Vector3 velocity;	// Velocity vector
	Vector3 axisOfRotation;	// Axis by which we rotate
	float angle;		// Angle we rotate by in radians
	Quaternion axes;	// Obb axes represented by a quarternion
	Vector3 halfWidths;	// Halfwidths for the vector
	MOVEMENT_PATTERN movementPattern;	// The style of movement which this will follow
	float t;	// The current "time"
	float tDir;	// Direction of time change
};

#endif

