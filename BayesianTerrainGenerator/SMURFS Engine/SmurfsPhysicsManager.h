/**************************************************************************************************
* Title: SmurfsPhysicsManager.h
* Authors: Gael Huber, Erin Mooney
* Description: Manages updating of physics
**************************************************************************************************/
#ifndef __PHYSICSMANAGER_H__
#define __PHYSICSMANAGER_H__

#include <vector>
#include "SmurfsRigidBody.h"
#include "SmurfsSingleton.h"
#include "SmurfsQuaternion.h"

class PhysicsManager : public Singleton<PhysicsManager> {
public:
	PhysicsManager(void);	// Constructor
	virtual ~PhysicsManager(void);	// Destructor

	void startUp(void);		// Initialize OpenGL
	void shutDown(void);	// Shutdown

	static PhysicsManager& getSingleton(void);	// Get the singleton instance
	static PhysicsManager* getSingletonPtr(void);	// Get the singleton pointer

	void update(void);	// Render the scene
	void addToWorld(RigidBody* obj);	// Add an object to the scene
	void removeFromWorld(void);	// Remove an object from the scene

private:
	std::vector<RigidBody*> bodyList;	// List of objects to update
};

#endif