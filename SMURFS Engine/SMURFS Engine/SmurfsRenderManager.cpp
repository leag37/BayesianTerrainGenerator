/**************************************************************************************************
* Title: RenderManager.cpp
* Authors: Gael Huber, Erin Mooney
* Description: Manages OpenGL rendering pipeline.
**************************************************************************************************/
#include "SmurfsRenderManager.h"

// Singleton instance
template<> RenderManager* Singleton<RenderManager>::singleton = 0;

/**************************************************************************************************
* Constructor
**************************************************************************************************/
RenderManager::RenderManager(void) {
	// Do nothing
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
RenderManager::~RenderManager(void) {
	// Do nothing
}

/**************************************************************************************************
* Initialize OpenGL
**************************************************************************************************/
void RenderManager::startUp(void) {
	// Initialize a new window
	_window = new Window();
	_window->startUp(L"SMURFS Engine", 1280, 720);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Set a black background
	shader = new Shader("..\\Shaders\\shader.vert", "..\\Shaders\\shader.frag");	// Initialize the shader

	// Create our perspective projection matrix
	projectionMatrix = glm::perspective(60.0f, (float)getWindowWidth()/(float)getWindowHeight(), 
		0.1f, 1000.0f);

	// Initialize the camera
	camera = new Camera();
	camera->init();
	camera->move(glm::vec3(0, 20.0f, 30.0f));

	glShadeModel(GL_SMOOTH);	// Enables smooth shading
	
	glClearDepth(1.0f);		// Depth buffer setup
	glEnable(GL_DEPTH_TEST);	// Enable depth testing
	glDepthFunc(GL_LEQUAL);	// The type of depth test
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations
}

/**************************************************************************************************
* Shutdown
**************************************************************************************************/
void RenderManager::shutDown(void) {
	// Destroy the window
	_window->shutDown();
	delete _window;
}

/**************************************************************************************************
* Get the singleton instance
**************************************************************************************************/
RenderManager& RenderManager::getSingleton(void) { 
	assert(singleton);
	return *singleton;
}

/**************************************************************************************************
* Get the singleton pointer
**************************************************************************************************/
RenderManager* RenderManager::getSingletonPtr(void) {
	return singleton;
}

/**************************************************************************************************
* Render the scene
**************************************************************************************************/
void RenderManager::renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	// Clear the screen

	camera->update();

	// Bind the shader
	shader->bind();
	// Get the locations of our matrices in the shader
	int projectionMatrixLocation = glGetUniformLocationARB(shader->getID(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shader->getID(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shader->getID(), "modelMatrix");

	// Send our matrices to the shader
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &(*camera->getViewMatrix())[0][0]);//cameraView[0][0]);

	// Draw to the screen
	std::vector<Mesh*>::iterator start = drawList.begin();
	std::vector<Mesh*>::iterator end = drawList.end();
	for(std::vector<Mesh*>::iterator itr = start; itr != end; ++itr) {
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &(*itr)->modelMatrix[0][0]);	// Bind the object's model matrix
		glBindVertexArray((*itr)->vaoID[0]);	// Bind our VAO

		// Render
		if((*itr)->mode == S_TRIANGLES)
			glDrawArrays(GL_TRIANGLES, 0, (*itr)->vertexCount / 3);	// Draw all triangles at once
		else
			glDrawArrays(GL_QUADS, 0, (*itr)->vertexCount / 3);	// Draw all quads at once

		glBindVertexArray(0);	// Unbind our VAO
	}

	// Unbind the shader
	shader->unbind();

	SwapBuffers(_window->getHDC());
}

/**************************************************************************************************
* Add an object to the scene
**************************************************************************************************/
void RenderManager::addToScene(Mesh* obj) {
	drawList.push_back(obj);
}

/**************************************************************************************************
* Remove an object from the scene
**************************************************************************************************/
void RenderManager::removeFromScene(GLuint id) {
}

/**************************************************************************************************
* Returns the camera object
**************************************************************************************************/
Camera* RenderManager::getCamera(void) { return camera; }

/**************************************************************************************************
* Returns the window
**************************************************************************************************/
Window* RenderManager::getWindow(void) { return _window; }