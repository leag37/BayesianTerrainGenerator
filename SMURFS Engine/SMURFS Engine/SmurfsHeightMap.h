//
// Title: SmurfsHeightMap.h
// Author: Gael Huber
// Description: Generate a height map of a given resolution with minimum and maximum height values
//
#ifndef __SMURFSHEIGHTMAP_H__
#define __SMURFSHEIGHTMAP_H__

#include <Windows.h>
#include <cmath>

#ifndef PI
#define PI 3.1415927f
#endif

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
	// Calculate perlin noise
	FLOAT calcPerlinNoise(FLOAT x, FLOAT z);

	// Calculate interpolated value for perlin noise
	FLOAT calcInterpolatedPerlinNoise(FLOAT x, FLOAT z);

	// Calculate smoothed noise for a given value
	FLOAT calcSmoothedPerlinNoise(FLOAT x, FLOAT z);

	// Interpolate between two values
	FLOAT calcInterpolation(FLOAT a, FLOAT b, FLOAT t);

	// Create random noise
	FLOAT calcRandomNoise(FLOAT x, FLOAT z);
	
	// Calculate the fade curve
	FLOAT calcFadeCurve(FLOAT t);

	// Calculate the gradient
	FLOAT calcGradient(INT hash, FLOAT x, FLOAT z); 

private:
	// Height map height data
	FLOAT** _heightMap;

	// Heightmap properties
	UINT _xWidth;
	UINT _zWidth;
	FLOAT _minHeight;
	FLOAT _maxHeight;

	// Perlin noise scale property
	FLOAT _perlinScale;
};

// Static related noise things
static UINT NOISE_PERMUTATION[256];
extern void SeedPermutation();

#endif // __SMURFSHEIGHTMAP_H__