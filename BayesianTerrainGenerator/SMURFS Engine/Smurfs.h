/**************************************************************************************************
* Title: Smurfs.h
* Authors: Gael Huber
* Description: Core file for the SMURFS (Super Mega Ultra Realistic Fyziks Simulation) Engine.
*	This engine will be expanded as a generalized physics engine built to handle 3D	physics.
**************************************************************************************************/
#ifndef __SMURFS_H__
#define __SMURFS_H__

#include "SmurfsSingleton.h"
#include "SmurfsGlobals.h"
#include "SmurfsRenderManager.h"

class Smurfs : public Singleton<Smurfs> {
public:
	Smurfs(void);	// Constructor
	~Smurfs(void);	// Destructor

	void startUp(void);	// Initialize the engine
	void shutDown(void);	// Shutdown the engine

	static Smurfs& getSingleton(void);	// Get singleton
	static Smurfs* getSingletonPtr(void);	// Get singleton pointer

	void update(void);	// Update
private:
	RenderManager* renderManager;	// Rendering
};

#endif