/**************************************************************************************************
* Title: InputManager.h
* Authors: Erin Mooney
* Description: Handles all mouse and keyboard input.
**************************************************************************************************/
#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include <Windows.h>
#include "SmurfsRenderManager.h"
#include "SmurfsVector2.h"
#include "SmurfsSingleton.h"
#include "SmurfsGlobals.h"
#include <time.h>

class InputManager : public Singleton<InputManager> {
public:
	InputManager(void);		// Constructor
	virtual ~InputManager(void);	// Destructor

	void startUp(void);		// Initialize
	void shutDown(void);	// Shutdown

	static InputManager& getSingleton(void);	// Get the singleton instance
	static InputManager* getSingletonPtr(void);	// Get the singleton pointer

	void handleInput(void);		// handles both mouse and keyboard input

	bool shouldAddChunk();

private:
	void handleKeyboardInput(void);		// handles all keyboard input
	void handleMouseInput(void);		// handles mouse input
	inline void checkScreenWrap(void);	// Handles mouse wrap with screen

	time_t prevTime;	// used for handling timing

	float lastMouseX;	// the previous mouse x position
	float lastMouseY;	// the previous mouse y position

	// TEMP
	bool _shouldAddChunk;
	bool _canAddChunk;
};

#endif