/**************************************************************************************************
* Title: SmurfsPhysicsManager.cpp
* Authors: Gael Huber, Erin Mooney
* Description: Manages updating of physics
**************************************************************************************************/
#include "SmurfsPhysicsManager.h"

// Singleton instance
template<> PhysicsManager* Singleton<PhysicsManager>::singleton = 0;

/**************************************************************************************************
* Constructor
**************************************************************************************************/
PhysicsManager::PhysicsManager(void) {
	// Do nothing
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
PhysicsManager::~PhysicsManager(void) {
	// Do nothing
}

/**************************************************************************************************
* Initialize physics
**************************************************************************************************/
void PhysicsManager::startUp(void) {

}

/**************************************************************************************************
* Shutdown
**************************************************************************************************/
void PhysicsManager::shutDown(void) {

}

/**************************************************************************************************
* Get the singleton instance
**************************************************************************************************/
PhysicsManager& PhysicsManager::getSingleton(void) { 
	assert(singleton);
	return *singleton;
}

/**************************************************************************************************
* Get the singleton pointer
**************************************************************************************************/
PhysicsManager* PhysicsManager::getSingletonPtr(void) {
	return singleton;
}

/**************************************************************************************************
* Update the scene
**************************************************************************************************/
void PhysicsManager::update(void) {
	// Define start and end of vector
	std::vector<RigidBody*>::iterator begin = bodyList.begin();
	std::vector<RigidBody*>::iterator end = bodyList.end();

	// Iterate through the list and update each object
	for(std::vector<RigidBody*>::iterator itr = begin; itr != end; ++itr) {
		// Apply velocity
		(*itr)->move();

		// Screen looping
		if((*itr)->position.x + (*itr)->halfWidths.x > 20.0f)
			(*itr)->position.x = -18.0f;
		else if((*itr)->position.x - (*itr)->halfWidths.x < -20.0f)
			(*itr)->position.x = 18.0f;

		if((*itr)->position.y + (*itr)->halfWidths.y > 20.0f)
			(*itr)->position.y = -18.0f;
		else if((*itr)->position.y - (*itr)->halfWidths.y < -20.0f)
			(*itr)->position.y = 18.0f;

		if((*itr)->position.z + (*itr)->halfWidths.z > 1.0f)
			(*itr)->position.z = -45.0f;
		else if((*itr)->position.z - (*itr)->halfWidths.z < -50.0f)
			(*itr)->position.z = (*itr)->halfWidths.z;

		// Apply angular velocity
		Quaternion q;
		q.w = cos((*itr)->angle / 2.0f);
		float s = sin((*itr)->angle / 2.0f);
		q.x = (*itr)->axisOfRotation.x * s;
		q.y = (*itr)->axisOfRotation.y * s;
		q.z = (*itr)->axisOfRotation.z * s;
		(*itr)->axes *= q;
	}
}

/**************************************************************************************************
* Add an object to the scene
**************************************************************************************************/
void PhysicsManager::addToWorld(RigidBody* obj) {
	bodyList.push_back(obj);
}

/**************************************************************************************************
* Remove an object from the scene
**************************************************************************************************/
void PhysicsManager::removeFromWorld(void) {
}