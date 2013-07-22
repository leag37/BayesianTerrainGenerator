/**************************************************************************************************
* Title: SimulationManager.h
* Authors: Gael Huber
* Description: Manages the actual game
**************************************************************************************************/
#ifndef __SIMULATIONMANAGER_H__
#define __SIMULATIONMANAGER_H__

#ifndef SMURFDEBUG
#if defined(_DEBUG) || defined(DEBUG)
#define SMURFDEBUG
#endif // _DEBUG || DEBUG
#endif // SMURFDEBUG

#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <sstream>
#include <vector>

#include "SmurfsEntity.h"
#include "SmurfsRenderManager.h"
#include "SmurfsPhysicsManager.h"
#include "SmurfsInputManager.h"
#include "SmurfsSingleton.h"
#include "SmurfsMatrixReader.h"
#include "SmurfsTerrain.h"

#define MAX_ENTITIES 750

class SimulationManager : public Singleton<SimulationManager> {
public:
	SimulationManager(void);	// Constructor
	~SimulationManager(void);	// Destructor

	void startUp(void);	// Initialize
	void shutDown(void);	// Shutdown

	static SimulationManager& getSingleton(void);	// Get the singleton instance
	static SimulationManager* getSingletonPtr(void);	// Get the singleton pointer

	void gameLoop(void);	// Update

	int getNumEntities(void);	// returns the number of entities in the world

private:
	void calcFPS();	// Calculate and log FPS

private:

	std::vector<Entity*> entityList;	// all entities on the screen

	// smurfs managers (singletons)
	RenderManager* renderManager;
	PhysicsManager* physicsManager;
	InputManager* inputManager;
	MatrixReader* matrixReader;
	Terrain* terrain;
	
	int numEntities;		// the number of entities in the world

	// Current number of frames
	unsigned int _numFrames;
	float _fps;
	Timer _timer;
	ostringstream _buff;
};

#endif