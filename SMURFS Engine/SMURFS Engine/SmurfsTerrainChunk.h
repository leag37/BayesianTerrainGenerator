//
// Title: SmurfsTerrainChunk.h
// Author: Gael Huber
// Description: Describes a chunk of terrain. Contains a renderable element for it and links to any
//	adjacent chunks

#ifndef __SMURFSTERRAINCHUNK_H__
#define __SMURFSTERRAINCHUNK_H__

#include "SmurfsMesh.h"
#include <Windows.h>

class TerrainChunk
{
public:

private:
	Mesh* _mesh;

	FLOAT _xWidth;
	FLOAT _zWidth;
};

#endif // __SMURFSTERRAINCUNK_H__
