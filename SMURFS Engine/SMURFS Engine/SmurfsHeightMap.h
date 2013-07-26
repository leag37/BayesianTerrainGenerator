//
// Title: SmurfsHeightMap.h
// Author: Gael Huber
// Description: Generate a height map of a given resolution with minimum and maximum height values
//
#ifndef __SMURFSHEIGHTMAP_H__
#define __SMURFSHEIGHTMAP_H__

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <Windows.h>
#include <cmath>
#include <limits>
#include "SmurfsMathUtils.h"
#include "SmurfsVector3.h"

#ifndef PI
#define PI 3.1415927f
#endif

class HeightMap
{
public:
	// Default constructor
	HeightMap();

	// Constructor
	HeightMap(
		FLOAT inXWidth, FLOAT inZWidth, 
		FLOAT inMinHeight, FLOAT inMaxHeight,
		FLOAT inStartX, FLOAT inStartZ,
		UINT inResolution);
	
	// Destructor
	~HeightMap();

	// Generate based on seeded values
	void generate();

	// Prefill the flowmap with other height maps
	void prefillFlowMap(HeightMap* hm);

	// Height map height data
	FLOAT** heightMap();

	// Flow map data
	Vector3** flowMap();

	// Heightmap properties
	UINT resolution();

	FLOAT minHeight();
	FLOAT maxHeight();
	FLOAT startX() { return _startX; }
	FLOAT startZ() { return _startZ; }
	const FLOAT startX() const { return _startX; }
	const FLOAT startZ() const { return _startZ; }

private:
	// Calculate perlin noise
	FLOAT calcPerlinNoise(FLOAT x, FLOAT y, FLOAT z, UINT numSamples);

	// Interpolate between two values
	FLOAT calcInterpolation(FLOAT a, FLOAT b, FLOAT t);

	// Create random noise
	FLOAT calcRandomNoise(FLOAT x, FLOAT y, FLOAT z);
	
	// Calculate the fade curve
	FLOAT calcFadeCurve(FLOAT t);

	// Calculate the gradient
	FLOAT calcGradient(INT hash, FLOAT x, FLOAT y, FLOAT z); 

	// Calculate the gradient based on integer values
	FLOAT calcGrad(INT hash, UINT x, UINT y, UINT z);

	// Seed the flow map with random unit vectors
	void seedFlowMap();

private:
	// Height map height data
	FLOAT** _heightMap;

	// Flow map with directional weights
	Vector3** _flowMap;

	// Heightmap properties
	UINT _resolution;
	FLOAT _minHeight;
	FLOAT _maxHeight;

	// Physical properties of map
	FLOAT _startX;
	FLOAT _startZ;
	
	FLOAT _xWidth;
	FLOAT _zWidth;
	
	// Perlin noise scale property
	FLOAT _perlinScale;
};

// Static related noise things
static UINT NOISE_PERMUTATION[512];
extern void SeedPermutation();

#endif // __SMURFSHEIGHTMAP_H__