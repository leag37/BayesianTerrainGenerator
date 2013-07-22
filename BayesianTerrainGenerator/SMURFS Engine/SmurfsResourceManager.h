/**************************************************************************************************
* Title: SmurfsResourceManager.h
* Authors: Gael Huber
* Description: Resource management system that permits local storage and retrieval of data.
*	This system will load in all folders pointed to by game configuration and keep track of game
*	resources.
**************************************************************************************************/
#ifndef __SMURFSRESOURCEMANAGER_H__
#define __SMURFSRESOURCEMANAGER_H__

#include "SmurfsSingleton.h"
#include <fstream>
#include <string>

using namespace std;

class ResourceManager : public Singleton<ResourceManager> {
public:
	ResourceManager(void);	// Constructor
	virtual ~ResourceManager(void);	// Destructor

	void startUp(void);		// Initializes sub-resource loaders and loads in all critical assets
	void shutDown(void);	// Shutdown

	static ResourceManager& getSingleton(void);	// Get the singleton instance
	static ResourceManager* getSingletonPtr(void);	// Get the singleton pointer

	void loadDataSet(const char* dataset);	// Load a set of data specified in a file, gives user 
		// flexibility and permits chunked loading
	void clearDataSet(const char* dataset);	// Delete the data from a particular data set in 

private:
	string root;	// Root directory
	string config;	// Config files directory
};

#endif
