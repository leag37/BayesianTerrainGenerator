/**************************************************************************************************
* Title: InputManager.cpp
* Authors: Gael Huber
* Description: Handles mouse and keyboard input.
**************************************************************************************************/
#include "SmurfsInputManager.h"
#include "SimulationManager.h"

// Singleton instance
template<> InputManager* Singleton<InputManager>::singleton = 0;

// Constructor
InputManager::InputManager(void) {
	time(&prevTime);

	// Initialize mouse position
	POINT mousePos;
	GetCursorPos(&mousePos);
	lastMouseX = (float) mousePos.x;
	lastMouseY = (float) mousePos.y;
}

// Destructor
InputManager::~InputManager(void) { }

/**************************************************************************************************
* Get the singleton instance
**************************************************************************************************/
InputManager& InputManager::getSingleton(void) { 
	assert(singleton);
	return *singleton;
}

/**************************************************************************************************
* Get the singleton pointer
**************************************************************************************************/
InputManager* InputManager::getSingletonPtr(void) {
	return singleton;
}

// Public Method to access both keyboard and mouse input
void InputManager::handleInput(void) {
	handleKeyboardInput();
	handleMouseInput();
}

// Handles keyboard input
void InputManager::handleKeyboardInput(void) {
	// Find the elapsed time
	time_t cTime;
	time(&cTime);
	time_t elapsedTime = cTime - prevTime;

	if((GetKeyState(VK_SPACE) & 0x80)) {
		// If enough time has elapsed, toggle paused
		if(elapsedTime >= 1) {
			toggleGamePaused();
			prevTime = cTime;
		}
	} 
	if((GetKeyState(0x57) & 0x80)) {		// W
		// move the camera forward (-forward vector)
		glm::vec3 moveForward = *RenderManager::getSingletonPtr()->getCamera()->getForwardVect();
		moveForward *= -1.0f;
		RenderManager::getSingletonPtr()->getCamera()->moveRelative(moveForward);
	}
	if((GetKeyState(0x41) & 0x80)) {		// A
		// move the camera left (+left vector)
		glm::vec3 moveLeft = *RenderManager::getSingletonPtr()->getCamera()->getLeftVect();
		moveLeft *= -1.0f;
		RenderManager::getSingletonPtr()->getCamera()->moveRelative(moveLeft);
	}
	if((GetKeyState(0x53) & 0x80)) {		// S
		// move the camera back (+forward vector)
		glm::vec3 moveBack = *RenderManager::getSingletonPtr()->getCamera()->getForwardVect();
		RenderManager::getSingletonPtr()->getCamera()->moveRelative(moveBack);
	}
	if((GetKeyState(0x44) & 0x80)) {		// D
		// move the camera right (-left vector)
		glm::vec3 moveRight = *RenderManager::getSingletonPtr()->getCamera()->getLeftVect();
		//moveRight *= -1.0f;
		RenderManager::getSingletonPtr()->getCamera()->moveRelative(moveRight);
	}
	if((GetKeyState(VK_ESCAPE) & 0x80)) {
		// Quit the application
		PostQuitMessage(0);
	}
}

// Handles mouse input
void InputManager::handleMouseInput(void) {
	// Check screen wrap
	//checkScreenWrap();
	
	// Find the current mouse positions
	POINT mousePos;
	GetCursorPos(&mousePos);
	float currentMouseX = (float) mousePos.x;
	float currentMouseY = (float) mousePos.y;

	// update yaw and pitch based on difference between current and previous mouse x and y, respectively
	RenderManager::getSingletonPtr()->getCamera()->pitch(currentMouseY - lastMouseY);
	RenderManager::getSingletonPtr()->getCamera()->yaw(currentMouseX - lastMouseX);

	// reset last mouse x and y positions
	lastMouseX = currentMouseX;
	lastMouseY = currentMouseY;
}

/**************************************************************************************************
* Handle mouse wrapping around the screen
**************************************************************************************************/
void InputManager::checkScreenWrap(void) {
	// Find the current location
	POINT mousePos;
	GetCursorPos(&mousePos);

	// Get the current screen rectangle
	RECT screen;
	GetWindowRect(RenderManager::getSingletonPtr()->getWindow()->getHWND(), &screen);

	// Check for overlap
	if(mousePos.x >= screen.right) {	// Right-side overlap
		// Set to left side of screen
		SetCursorPos(screen.left + 1, mousePos.y);
		lastMouseX = (float) (screen.left + 1.0f);
	} else if(mousePos.x <= screen.left) {	// Left-side overlap
		// Set to right side of screen
		SetCursorPos(screen.right - 1, mousePos.y);
		lastMouseX = (float) (screen.right - 1.0f);
	}

	if(mousePos.y >= screen.bottom) {	// Bottom-side overlap
		// Set to top side of screen
		SetCursorPos(mousePos.x, screen.top + 1);
		lastMouseY = (float) (screen.top + 1.0f);
	} else if(mousePos.y <= screen.top) {	// Top-side overlap
		// Set to bottom side of screen
		SetCursorPos(mousePos.x, screen.bottom - 1);
		lastMouseY = (float) (screen.bottom - 1.0f);
	}
}