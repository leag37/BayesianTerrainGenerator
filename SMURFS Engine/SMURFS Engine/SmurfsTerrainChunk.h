//
// Title: SmurfsTerrainChunk.h
// Author: Gael Huber
// Description: Describes a chunk of terrain. Contains a renderable element for it and links to any
//	adjacent chunks

#ifndef __SMURFSTERRAINCHUNK_H__
#define __SMURFSTERRAINCHUNK_H__

#include "SmurfsMesh.h"

class TerrainChunk
{
public:

private:
	Mesh* _mesh;

	float _xWidth;
	float _zWidth;
};

#endif // __SMURFSTERRAINCUNK_H__
