/**************************************************************************************************
* Title: RenderManager.h
* Authors: Gael Huber, Erin Mooney
* Description: Manages OpenGL rendering pipeline.
**************************************************************************************************/
#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include <time.h>
#include <vector>
#include <math.h>
#include "SmurfsCamera.h"
#include "SmurfsMesh.h"
#include "SmurfsSingleton.h"
#include "SmurfsWindow.h"
#include "SmurfsShader.h"
#include "OpenGL/glm-0.9.1.1/glm.hpp"
#include "OpenGL/glm-0.9.1.1/gtc/matrix_transform.hpp"

class RenderManager : public Singleton<RenderManager> {
public:
	RenderManager(void);	// Constructor
	virtual ~RenderManager(void);	// Destructor

	void startUp(void);		// Initialize OpenGL
	void shutDown(void);	// Shutdown

	static RenderManager& getSingleton(void);	// Get the singleton instance
	static RenderManager* getSingletonPtr(void);	// Get the singleton pointer

	void renderScene(void);	// Render the scene
	void addToScene(Mesh* obj);	// Add an object to the scene
	void removeFromScene(GLuint id);	// Remove an object from the scene

	std::vector<Mesh> objectList;	// The list of premade objects to draw from
	
	// getter for camera
	Camera* getCamera(void);		// returns the camera =)
	Window* getWindow(void);		// Returns the window

private:
	void buildDisplayLists(void);	// Build the display lists
	void buildFont(void);			// Build a bitmap font
	void killFont(void);			// Destroys the font

	void print(const char* toPrint);	// Prints out stuff :)

	Window* _window;	// The game window
	Shader* shader;		// Shader for rendering
	std::vector<Mesh*> drawList;	// List of objects to draw

	glm::mat4 projectionMatrix; // Store the projection matrix
	Camera* camera;		// Camera!
	glm::mat4 viewMatrix; // Store the view matrix	TODO: Replace with camera

};

#endif