//*************************************************************************************************
// Title: SmurfsTerrainMap.h
// Author: Gael Huber
// Description: Data structure for holding the entirety of the terrain. 
//*************************************************************************************************
#ifndef __SMURFSTERRAINMAP_H__
#define __SMURFSTERRAINMAP_H__

#include "SmurfsTerrainChunk.h"
#include "SmurfsHeightMap.h"
#include <vector>

class TerrainMap
{
public:
	// Default constructor
	TerrainMap();

	TerrainMap(
		FLOAT inMinHeight, FLOAT inMaxHeight, 
		FLOAT inChunkXWidth, FLOAT inChunkZWidth, 
		UINT inChunkResolution);

	void addChunk();
	std::vector<Mesh*>& addToRenderList() { return _addToRenderList; }

private:
	// Add a chunk at a given starting position, seeding with adjacent height maps
	void addChunkAt(FLOAT x, FLOAT z);

private:
	// List of terrain chunks
	std::vector<TerrainChunk*> _chunkList;

	// Chunks that need to be added to render list
	std::vector<Mesh*> _addToRenderList;

	// World dimensions
	FLOAT _minHeight;
	FLOAT _maxHeight;
	
	FLOAT _xWidth;
	FLOAT _zWidth;

	// Chunk dimensions
	FLOAT _chunkXWidth;
	FLOAT _chunkZWidth;
	UINT _chunkResolution;

	// Variables for initializing new chunks
	// Target number of chunks in x direction per row
	UINT _targetChunksX;

	// Target number of chunks in z direction per row
	UINT _targetChunksZ;
};

#endif // __SMURFSTERRAINMAP_H__