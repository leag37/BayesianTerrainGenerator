/**************************************************************************************************
* Title: SimulationManager.cpp
* Authors: Gael Huber, Erin Mooney
* Description: Manages the actual game
**************************************************************************************************/
#include "SimulationManager.h"

// Singleton instance
template<> SimulationManager* Singleton<SimulationManager>::singleton = 0;

// Constructor
SimulationManager::SimulationManager(void) : numEntities(0) {
}	

// Destructor
SimulationManager::~SimulationManager(void) {
}

// Initialize
void SimulationManager::startUp(void) {
	// random seed
	srand((unsigned int) time(0));

	// create the input manager
	inputManager = new InputManager();
	
	// create the render manager
	renderManager = new RenderManager();
	renderManager->startUp();

	// create physics manager
	physicsManager = new PhysicsManager();

	// Initialize the matrix reader
	matrixReader = new MatrixReader();

	// Create and generate the terrain
	terrain = new Terrain();
	terrain->generateTerrain();
	renderManager->addToScene(terrain->getMesh());

	// TEMP
	_numFrames = 0;
	_fps = 0.0f;
	
	// Setup a timer
#ifdef SMURFDEBUG
	_timer.setupTimer();
#endif
}

// Shutdown
void SimulationManager::shutDown(void) {
}

/**************************************************************************************************
* Get the singleton instance
**************************************************************************************************/
SimulationManager& SimulationManager::getSingleton(void) { 
	assert(singleton);
	return *singleton;
}

/**************************************************************************************************
* Get the singleton pointer
**************************************************************************************************/
SimulationManager* SimulationManager::getSingletonPtr(void) {
	return singleton;
}

// Update
void SimulationManager::gameLoop(void) {
	MSG msg;	// Windows message
	BOOL done = FALSE;	// Bool variable to exit loop

	while( !done ) {

#ifdef SMURFDEBUG
		// Query for FPS
		calcFPS();
#endif

		// Is there a message waiting?
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
			// Have we received a quit message?
			if( msg.message == WM_QUIT ) {
				done = TRUE;	// If so, we want to quit
			} else {
				// Otherwise deal with Windows messages
				TranslateMessage(&msg);	// Translate the message
				DispatchMessage(&msg);	// Dispatch the message
			}
		} else { // If there is no message
			// handle input
			inputManager->handleInput();

			// Draw the scene
			renderManager->renderScene();
		}
	}
}

// getter for the number of entities in the world
int SimulationManager::getNumEntities(void) { return numEntities; }

// Calculate FPS
void SimulationManager::calcFPS()
{
	// Increment frames
	++_numFrames;

	// Get the current tick
	_fps += _timer.getDeltaTime();

	// Exceeded one frame
	if(_fps >= 1.0f)
	{
		// Log fps
		_buff << "FPS: ";
		_buff << _numFrames;
		_buff << endl;
		OutputDebugStringA((LPCSTR) _buff.str().c_str());
		_buff.clear();
		_buff.str("");
		_numFrames = 0;
		_fps = 0.0f;
	}
}