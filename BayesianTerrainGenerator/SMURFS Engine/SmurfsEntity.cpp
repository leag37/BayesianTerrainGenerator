/**************************************************************************************************
* Title: SmurfsEntity.cpp
* Author: Gael Huber
* Description: Describes a game object, which contains linkages to various other components in
*	the engine and manages its relation to those components. 
**************************************************************************************************/
#include "SmurfsEntity.h"

UINT64 Entity::nextAvailableID = 0;

/**************************************************************************************************
* Constructor
**************************************************************************************************/
Entity::Entity(void) {
	mesh = new Mesh();	// Initialize
	rigidBody = new RigidBody();	// Create the object's rigid body
	assignNextID();	// Assign the object the next available unique ID
}

/**************************************************************************************************
* Constructor
*
* Mesh* mesh	Mesh that coincides to the entity
**************************************************************************************************/
Entity::Entity(Mesh* mesh, RigidBody* body) {
	this->mesh = mesh;	// Assign the mesh

	// Create the object's rigid body
	rigidBody = body;	

	assignNextID();	// Assign the object the next available unique ID
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
Entity::~Entity(void) {
	// Delete primary components
	delete mesh;
	mesh = 0;

	delete rigidBody;
	rigidBody = 0;
}

/**************************************************************************************************
* Update that happens post-physics. This updates the mesh position and orientation based on the 
* rigid body.
**************************************************************************************************/
void Entity::postPhysicsUpdate(void) {	
	// Find the amount by which to translate the object
	glm::vec3 translate = glm::vec3(rigidBody->position.x, rigidBody->position.y, rigidBody->position.z);
	// Translate the object
	mesh->modelMatrix = glm::translate(glm::mat4(1.0f), translate);
	
	// Update modelMatrix rotate/scale components
	//************************************************************************************************
	// Convert the quaternion of the rigid body representing rotation to a matrix
	Matrix m = rigidBody->axes.toMatrix();

	// Reset the model matrix
	static float angle = 0.0f;
	angle += 0.01f;
	Quaternion a1 = Quaternion(angle, Vector3(0, 1.0f, 1.0f).getNormalized());
	m = rigidBody->axes.toMatrix();

	mesh->modelMatrix[0][0] = m.matrix[0][0];
	mesh->modelMatrix[0][1] = m.matrix[0][1];
	mesh->modelMatrix[0][2] = m.matrix[0][2];

	mesh->modelMatrix[1][0] = m.matrix[1][0];
	mesh->modelMatrix[1][1] = m.matrix[1][1];
	mesh->modelMatrix[1][2] = m.matrix[1][2];

	mesh->modelMatrix[2][0] = m.matrix[2][0];
	mesh->modelMatrix[2][1] = m.matrix[2][1];
	mesh->modelMatrix[2][2] = m.matrix[2][2];

	// Find the scale of the object
	glm::vec3 scale = glm::vec3(rigidBody->halfWidths.x, rigidBody->halfWidths.y, rigidBody->halfWidths.z);

	// Scale the object
	//mesh->modelMatrix = glm::scale(mesh->modelMatrix, scale);	//mesh->modelMatrix, scale);

	// Update the screen coordinates
	mesh->screenCoords = rigidBody->position;
}

/**************************************************************************************************
* Assign the next available unique ID
**************************************************************************************************/
void Entity::assignNextID(void) {
	uniqueID = ++nextAvailableID;
}

// return the rigid body (I know I shouldn't do this... but I want to modify things based on input...)
RigidBody* Entity::getBody(void) { return rigidBody; }