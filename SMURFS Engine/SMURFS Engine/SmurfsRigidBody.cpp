/**************************************************************************************************
* Title: SmurfsRigidBody.cpp
* Author: Gael Huber
* Description: Holds a rigid body used for physics and collision systems.
**************************************************************************************************/
#include "SmurfsRigidBody.h"

/**************************************************************************************************
* Constructor
**************************************************************************************************/
RigidBody::RigidBody(void) {
	srand((unsigned int) time(0));
	t = 0;

	// Determine a random movement pattern
	int mp = rand() % 10;
	if(mp < 4)
		movementPattern = STANDARD;
	else if(mp >= 4 && mp < 6)
		movementPattern = HELIX;
	else if(mp >= 6 && mp < 8)
		movementPattern = PARABOLA;
	else if(mp >= 8)
		movementPattern = CIRCLE;

	// Initialize the axes
	axes = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

/**************************************************************************************************
* Constructor defining dimensions
**************************************************************************************************/
RigidBody::RigidBody(Vector3 axes, Vector3 halfWidths) {	
	// Determine a random movement pattern
	int mp = rand() % 10;
	if(mp < 4)
		movementPattern = STANDARD;
	else if(mp >= 4 && mp < 6)
		movementPattern = HELIX;
	else if(mp >= 6 && mp < 8)
		movementPattern = PARABOLA;
	else if(mp >= 8)
		movementPattern = CIRCLE;

	this->axes.x = axes.x;
	this->axes.y = axes.y;
	this->axes.z = axes.z;
	this->halfWidths = halfWidths;

	t = 0;
	tDir = -1.0f;
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
RigidBody::~RigidBody(void) {
}

/**************************************************************************************************
* Move the rigid body according to it's particularly movement pattern
**************************************************************************************************/
void RigidBody::move(void) {
	// Time wrap
	if(t > 18.0f)
		tDir *= -1;
	if(t < -18.0f)
		tDir *= -1;

	switch(movementPattern) {
	// Standard movement pattern
	case STANDARD:
		position += velocity;
		break;

	// Helix movement pattern
	case HELIX:
		// Increment "time"
		t += 0.1f * tDir;
		
		position.x += cos(t) / 2.0f;
		position.y += sin(t) / 2.0f;
		position.z += t * 0.04f;

		// standard movement as well
		position += velocity;
		break;

	// Parabolic movement pattern
	case PARABOLA:
		// Increment "time"
		t += 0.2f * tDir;

		position.y += (t / abs(t)) * t*t / 2000.0f;

		// standard movement as well
		position += velocity;
		break;

	// Circular movement pattern
	case CIRCLE:
		// Increment "time"
		t += 0.1f * tDir;
		
		position.x += cos(t) / 5.0f;
		position.y += sin(t) / 5.0f;

		// standard movement as well
		position += velocity;
		break;
	}
		
}