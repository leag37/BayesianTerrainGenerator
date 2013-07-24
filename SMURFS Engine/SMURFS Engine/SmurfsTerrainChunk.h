//
// Title: SmurfsTerrainChunk.h
// Author: Gael Huber
// Description: Describes a chunk of terrain. Contains a renderable element for it and links to any
//	adjacent chunks

#ifndef __SMURFSTERRAINCHUNK_H__
#define __SMURFSTERRAINCHUNK_H__

#include "SmurfsMesh.h"
#include "SmurfsVector3.h"
#include "SmurfsHeightMap.h"
#include <Windows.h>

class TerrainChunk
{
public:
	TerrainChunk();

	TerrainChunk(HeightMap* hm);

	Mesh* mesh();

	// Generate a mesh from a height map
	void genMeshFromHeightMap();

private:
	Mesh* _mesh;

	// Heightmap for this terrain chunk
	HeightMap* _hm;

	// X width of the chunk
	FLOAT _xWidth;

	// Z width of the chunk
	FLOAT _zWidth;

	// Starting x of the chunk
	FLOAT _xStart;

	// Starting z of the chunk
	FLOAT _zStart;


	// TEMP
	Vector3 snow;
	Vector3 rock;
	Vector3 grass;
	Vector3 sand;
	Vector3 water;
	
	Vector3 assignColor(FLOAT x, FLOAT y, FLOAT z);
	//END TEMP
};

#endif // __SMURFSTERRAINCUNK_H__
