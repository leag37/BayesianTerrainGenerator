//
// Title: SmurfsHeightMap.cpp
// Author: Gael Huber
// Description: Generate a height map of a given resolution with minimum and maximum height values
//
#include "SmurfsHeightMap.h"

// Default constructor
HeightMap::HeightMap()
	: _heightMap(0)
{ }

// Default constructor
HeightMap::HeightMap(UINT inXWidth, UINT inZWidth, FLOAT inMinHeight, FLOAT inMaxHeight)
	:	_heightMap(0), 
		_xWidth(inXWidth), _zWidth(inZWidth),
		_minHeight(inMinHeight), _maxHeight(inMaxHeight)
{ 
	// Create height map
	_heightMap = new FLOAT*[_xWidth];
	for(UINT i = 0; i < _xWidth; ++i)
	{
		_heightMap[i] = new FLOAT[_zWidth];
		for(UINT j = 0; j < _zWidth; ++j)
		{
			_heightMap[i][j] = 0.0f;
		}
	}
}

HeightMap::~HeightMap()
{

}

void HeightMap::generate()
{
	// 


}



void SeedPermutation()
{
	// Initialize the permutation
	for(UINT i = 0; i < 256; ++i)
	{
		NOISE_PERMUTATION[i] = 0xFFFFFFFF;
	}

	// We must seed every number from 0 to 255
	for(UINT i = 0; i < 256; ++i)
	{
		// Generate an initial index
		UINT index = rand() % 256;
		
		// This is a valid index, insert
		if(NOISE_PERMUTATION[index] == 0xFFFFFFFF)
		{
			NOISE_PERMUTATION[index] = i;
		}
		// Seek first available index
		else
		{
			BOOL seekInsertionPoint = true;
			while(seekInsertionPoint)
			{
				// Seek next index
				++index;

				// Wrap index
				if(index == 256)
				{
					index = 0;
				}

				// Found a valid index, choose
				if(NOISE_PERMUTATION[index] == 0xFFFFFFFF)
				{
					NOISE_PERMUTATION[index] = i;
					seekInsertionPoint = false;
				}
			}
		}
	}
}