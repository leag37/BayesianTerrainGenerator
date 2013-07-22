/**************************************************************************************************
* Title: Camera.h
* Author(s): Erin Mooney
* Description: Creates a first-person "camera" to look around the world
**************************************************************************************************/
#ifndef __SMURFSCAMERA_H__
#define __SMURFSCAMERA_H__

#include "OpenGL/glm-0.9.1.1/glm.hpp"
#include "OpenGL/glm-0.9.1.1/gtc/matrix_transform.hpp"
#include "SmurfsMathConstants.h"
#include "SmurfsVector3.h"
#include "SmurfsTimer.h"

class Camera {
public:
	Camera(void);		// constructor
	~Camera(void);		// destructor

	void init(void);	// initializes the camera

	void update(void);	// updates the camera each frame

	void updateVectors(void);	// updates the forward and left vectors

	void move(glm::vec3 moveBy);			// move the camera relative to the world axis
	void moveRelative(glm::vec3 moveBy);		// move the camera relative to the local axis

	void yaw(float degrees);		// rotate the camera by yaw
	void pitch(float degrees);		// rotate the camera by pitch
	void roll(float degrees);		// rotate the camera by roll

	glm::vec3* getPosition(void);		// Return position vector
	glm::vec3* getForwardVect(void);	// Returns the forward vector for movement
	glm::vec3* getLeftVect(void);		// Returns the left vector for movement
	glm::mat4* getViewMatrix(void);		// Returns the view matrix for rendering
	glm::vec3* getRotation(void);		// Return rotation vector

private:
	Timer timer;		// timer for getting delta time	
	float deltaTime;	// delta time in seconds

	glm::vec3 position;			// the position of the camera
	glm::vec3 rotationAngle;	// the angle that the camera rotates by
	glm::mat4 viewMatrix;		// the camera's orientation

	float moveSpeed;			// the speed of the camera movement
	float rotationSpeed;		// the speed of the camera's rotation

	Vector3 forward;			// movement in the forward direction based on angle
	Vector3 left;				// movement in the left direction based on angle
};

#endif