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

			// Run if the game is not paused
			if(!getGamePaused()) {
				// Update physics
				physicsManager->update();

				// Post-physics update
				// Define start and end of vector
				std::vector<Entity*>::iterator begin = entityList.begin();
				std::vector<Entity*>::iterator end = entityList.end();

				// Iterate through the list and update each object
				for(std::vector<Entity*>::iterator itr = begin; itr != end; ++itr) {
					// Apply update
					(*itr)->postPhysicsUpdate();
				}
			}

			// Draw the scene
			renderManager->renderScene();
		}
	}
}

// getter for the number of entities in the world
int SimulationManager::getNumEntities(void) { return numEntities; }