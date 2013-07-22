/***************************************************************************
* Title: TerrainLoader.h
* Author(s): Erin Mooney
* Description: Reads in information about creating the terrain from a file
***************************************************************************/
#ifndef __TERRAINLOADER_H__
#define __TERRAINLOADER_H__

#include <stdlib.h>

#include <string>
#include <fstream>
#include <vector>

class TerrainLoader {
public:
	TerrainLoader(void);		// constructor
	~TerrainLoader(void);		// destructor

	std::vector<float>* load(std::string filename);	// reads the file and sets the correct variables
private:
	std::string FILEPATH;		// the filepath to the terrain folder
};

#endif