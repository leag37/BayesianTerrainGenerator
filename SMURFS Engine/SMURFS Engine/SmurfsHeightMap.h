//
// Title: SmurfsHeightMap.h
// Author: Gael Huber
// Description: Generate a height map of a given resolution with minimum and maximum height values
//
#ifndef __SMURFSHEIGHTMAP_H__
#define __SMURFSHEIGHTMAP_H__

#include <Windows.h>

class HeightMap
{
public:
	// Default constructor
	HeightMap();

	// Constructor
	HeightMap(UINT inXWidth, UINT inZWidth, FLOAT inMinHeight, FLOAT inMaxHeight);

	// Destructor
	~HeightMap();

	// Generate based on seeded values
	void generate();

private:
	// Height map height data
	FLOAT** _heightMap;

	// Heightmap properties
	UINT _xWidth;
	UINT _zWidth;
	FLOAT _minHeight;
	FLOAT _maxHeight;
};

// Static related noise things
static UINT NOISE_PERMUTATION[256];
extern void SeedPermutation();

#endif // __SMURFSHEIGHTMAP_H__