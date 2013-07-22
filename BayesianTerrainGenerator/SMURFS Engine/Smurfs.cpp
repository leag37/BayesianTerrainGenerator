/**************************************************************************************************
* Title: Smurfs.cpp
* Authors: Gael Huber
* Description: Core file for the SMURFS (Super Mega Ultra Realistic Fyziks Simulation) Engine.
*	This engine will be expanded as a generalized physics engine built to handle 3D	physics.
**************************************************************************************************/
#include "Smurfs.h"

template<> Smurfs* Singleton<Smurfs>::singleton = 0;

Smurfs::Smurfs(void) {
	// Do nothing
}

Smurfs::~Smurfs(void) {
	// Do nothing
}

void Smurfs::startUp(void) {
	//renderManager = new RenderManager();
	//renderManager->startUp();
}

void Smurfs::shutDown(void) {
	// Do nothing
}

void Smurfs::update(void) {
}