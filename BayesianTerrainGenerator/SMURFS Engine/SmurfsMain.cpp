/**************************************************************************************************
* Title: Pong.cpp
* Authors: Gael Huber
* Description: Main method for simple Pong game.
**************************************************************************************************/
#include "Smurfs.h"
#include "SimulationManager.h"

/**************************************************************************************************
* Main
*
* HINSTANCE	Instance
* HINSTANCE Previous instance
* LPSTR	Command Line Parameters
* int	Window show state
**************************************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Prepare the engine
	// Prepare the game
	// Setup each to run on a thread
	// Engine just updates it's apparent world
	// The only thing that the game may do is contribute to the engine's "scratch pad"
	// To contribute to the scratchpad, adds to a message which indicates how the world should be changed at the end of the next update
	
	// Initialize the SMURFS engine
	Smurfs* smurfs = new Smurfs();
	smurfs->startUp();

	// Initialize the game, this will interface with the SMURFS engine
	SimulationManager* simulationManager = new SimulationManager();
	simulationManager->startUp();

	// Run the game
	simulationManager->gameLoop();

	return 0;
}