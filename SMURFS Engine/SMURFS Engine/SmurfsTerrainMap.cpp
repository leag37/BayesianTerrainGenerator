#include "SmurfsTerrainMap.h"

// Default constructor
TerrainMap::TerrainMap()
	:	_minHeight(0.0f), _maxHeight(0.0f),
		_chunkXWidth(0.0f), _chunkZWidth(0.0f),
		_chunkResolution(0)
{ }

TerrainMap::TerrainMap(
	FLOAT inMinHeight, FLOAT inMaxHeight, 
	FLOAT inChunkXWidth, FLOAT inChunkZWidth, 
	UINT inChunkResolution)
	:	_minHeight(inMinHeight), _maxHeight(inMaxHeight),
		_chunkXWidth(inChunkXWidth), _chunkZWidth(inChunkZWidth),
		_chunkResolution(inChunkResolution)
{ 
	// New map, no chunks in any direction
	_targetChunksX = 0;
	_targetChunksZ = 0;
}

void TerrainMap::addChunk()
{
	// Check whether we need to attempt to expand our target chunk dimensions
	if(_chunkList.size() == _targetChunksX * _targetChunksZ)
	{
		_targetChunksX = ++_targetChunksZ;

		FLOAT startX = static_cast<FLOAT>(_targetChunksX - 1) * _chunkXWidth;
		FLOAT startZ = 0.0f;

		addChunkAt(startX, startZ);
	}
	else
	{
		// Choose target x and target z values
		FLOAT targetX = static_cast<FLOAT>(_targetChunksX - 1) * _chunkXWidth;
		FLOAT targetZ = static_cast<FLOAT>(_targetChunksZ - 1) * _chunkZWidth;
		FLOAT chooseX = 0.0f;
		FLOAT chooseZ = 0.0f;

		// Now, we must loop through and determine the smallest possible x and z values for our starting points
		// Iterate through the list
		std::vector<TerrainChunk*>::iterator end = _chunkList.end();
		for(std::vector<TerrainChunk*>::iterator itr = _chunkList.begin(); itr != end; ++itr)
		{
			// Check if this chunk is along the z border
			if((*itr)->startX() == targetX && (*itr)->startZ() >= chooseZ)
			{
				// Increment startZ
				chooseZ = (*itr)->startZ() + _chunkZWidth;
			}
			else if((*itr)->startZ() == targetZ && (*itr)->startX() >= chooseX)
			{
				chooseX = (*itr)->startX() + _chunkXWidth;	
			}
		}

		// Pick x and z such that
		//	if(sxz < szx) choose sx, sxz
		//	if(szx < sxz) choose szx, sz
		//	if(sxz == szx) choose sx, sxz
		FLOAT startX, startZ = 0.0f;
		if(chooseX < chooseZ)
		{
			startX = chooseX;
			startZ = targetZ;
		}
		else
		{
			startX = targetX;
			startZ = chooseZ;
		}

		// Create chunk
		addChunkAt(startX, startZ);
	}
}

// Add a chunk at a given starting position, seeding with adjacent height maps
void TerrainMap::addChunkAt(FLOAT x, FLOAT z)
{
	// Create the chunk
	HeightMap* hm = new HeightMap(_chunkXWidth, _chunkZWidth, _minHeight, _maxHeight, x, z, _chunkResolution);
	
	// Iterate through the map and find adjacent terrain
	std::vector<TerrainChunk*>::iterator end = _chunkList.end();
	for(std::vector<TerrainChunk*>::iterator itr = _chunkList.begin(); itr != end; ++itr)
	{
		// If startx + 1 && startz are equal OR if startz + 1 && startx are equal
		FLOAT hmSX = (*itr)->heightMap()->startX();
		FLOAT hmSX1 = hmSX + _chunkXWidth;
		FLOAT hmSZ = (*itr)->heightMap()->startZ();
		FLOAT hmSZ1 = hmSZ + _chunkZWidth;
		if((x == hmSX1 && z == hmSZ) || (x == hmSX && z == hmSZ1))
		{
			hm->prefillFlowMap((*itr)->heightMap());
		}
	}

	// We have prefilled, so now we may create the height map
	hm->generate();
	TerrainChunk* chunk = new TerrainChunk(hm);
	chunk->genMeshFromHeightMap();
	
	// Push this onto the list
	_chunkList.push_back(chunk);

	// Add to the "needs to get added to render system" list
	_addToRenderList.push_back(chunk->mesh());
}


