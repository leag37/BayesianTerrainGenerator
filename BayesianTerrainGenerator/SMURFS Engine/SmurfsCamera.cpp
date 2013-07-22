/**************************************************************************************************
* Title: Camera.cpp
* Author(s): Erin Mooney
* Description: Creates a first-person "camera" to look around the world
**************************************************************************************************/
#include "SmurfsCamera.h"

/**************************************************************************************************
* Constructor
**************************************************************************************************/
Camera::Camera(void) {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
	viewMatrix = glm::mat4(1.0f);

	moveSpeed = 1000.0f;
	rotationSpeed = 20.0f;

	forward = Vector3(0.0f, 0.0f, 0.0f);
	left = Vector3(0.0f, 0.0f, 0.0f);
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
Camera::~Camera(void) { }

/**************************************************************************************************
* Initializes the camera
**************************************************************************************************/
void Camera::init(void) { 
	timer = Timer();
	timer.setupTimer();
	timer.setStartTime();
}

/**************************************************************************************************
* Updates the camera each frame
**************************************************************************************************/
void Camera::update(void) {
	// get delta time
	deltaTime = timer.getDeltaTime();

	// update view matrix
	viewMatrix = glm::mat4(1.0f);
	// rotate the matrix
	viewMatrix *= glm::rotate(glm::mat4(1.0f), rotationAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));
	viewMatrix *= glm::rotate(glm::mat4(1.0f), rotationAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	viewMatrix *= glm::rotate(glm::mat4(1.0f), rotationAngle.z, glm::vec3(0.0f, 0.0f, 1.0f));
	// translate the matrix
	viewMatrix *= glm::translate(glm::mat4(1.0f), -position);
}

/**************************************************************************************************
* Updates the forward and left vectors
**************************************************************************************************/
void Camera::updateVectors(void) { 
	// update the forward vector => x = sin(-angle.y), y = sin(angle.x), z = cos(-angle.y)
	forward = Vector3((float)sin(-rotationAngle.y / 180.0f * PI), (float)sin(rotationAngle.x / 180.0f * PI), (float)cos(-rotationAngle.y / 180.0f * PI));
	forward.normalize();

	// update the left vector => x = cos(angle.y), y = 0, z = sin(angle.y)
	left = Vector3((float)cos(rotationAngle.y / 180.0f * PI), 0.0f, (float)sin(rotationAngle.y / 180.0f * PI));
	left.normalize();
}

/**************************************************************************************************
* Moves the camera relative to the world axis
**************************************************************************************************/
void Camera::move(glm::vec3 moveBy) {
	position += moveBy;
}

/**************************************************************************************************
* Moves the camera relative to the local axis
**************************************************************************************************/
void Camera::moveRelative(glm::vec3 moveBy) {
	position += moveBy * moveSpeed * deltaTime;
}

/**************************************************************************************************
* Rotate the camera by yaw (y axis)
**************************************************************************************************/
void Camera::yaw(float degrees) {
	rotationAngle.y += degrees * rotationSpeed * 0.01f;
	updateVectors();
}

/**************************************************************************************************
* Rotate the camera by pitch (x axis)
**************************************************************************************************/
void Camera::pitch(float degrees) {
	rotationAngle.x += degrees * rotationSpeed * 0.01f;
	updateVectors();
}

/**************************************************************************************************
* Rotate the camera by roll (z axis)
**************************************************************************************************/
void Camera::roll(float degrees) {
	rotationAngle.z += degrees * rotationSpeed * 0.01f;
}

/**************************************************************************************************
* Getters
**************************************************************************************************/
glm::vec3* Camera::getPosition(void) { return &position; }
glm::vec3* Camera::getForwardVect(void) { return new glm::vec3(forward.x, forward.y, forward.z); }
glm::vec3* Camera::getLeftVect(void) { return new glm::vec3(left.x, left.y, left.z); }
glm::mat4* Camera::getViewMatrix(void) { return &viewMatrix; }