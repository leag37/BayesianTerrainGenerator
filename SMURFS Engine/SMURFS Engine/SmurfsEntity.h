/**************************************************************************************************
* Title: SmurfsEntity.h
* Author: Gael Huber
* Description: Describes a game object (entity), which contains linkages to various other 
*	components in the engine and manages its relation to those components. 
**************************************************************************************************/
#ifndef __SMURFSENTITY_H__
#define __SMURFSENTITY_H__

#include "SmurfsRigidBody.h"
#include "SmurfsMesh.h"
#include "SmurfsVector3.h"
#include "SmurfsMatrix.h"
#include "SmurfsQuaternion.h"
#include <Windows.h>

class Entity {
public:
	Entity(void);	// Constructor
	Entity(Mesh* object, RigidBody* body);	// Pass in a mesh, define the object's rigid body based on the mesh
	~Entity(void);	// Destructor

	void postPhysicsUpdate(void);	// Update that happens post-physics. This updates the mesh position
		// and orientation based on the rigid body.

	RigidBody* getBody(void);	// Returns the rigid body of the entity
private:
	void assignNextID(void);	// Assign the next available unique ID

	Mesh* mesh;	// Entity render mesh 
	RigidBody* rigidBody;	// Rigid body for physics
	Vector3 location;	// Entity location in the game world, this is abstracted, but related to, the
		// location of its mesh and physics body

	UINT64 uniqueID;	// Entity unique ID
	static UINT64 nextAvailableID;	// The next available unique ID
};

#endif