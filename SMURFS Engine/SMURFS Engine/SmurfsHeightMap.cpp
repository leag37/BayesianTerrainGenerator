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

	// Set perlin scale
	_perlinScale = 0.01f;
}

HeightMap::~HeightMap()
{

}

// Generate the height map
void HeightMap::generate()
{
	// Calculate range
	FLOAT range = _maxHeight - _minHeight;

	// Iterate through the height map and generate noise for each coordinate
	for(UINT i = 0; i < _xWidth; ++i)
	{
		for(UINT j = 0; j < _zWidth; ++j)
		{
			FLOAT val = calcPerlinNoise(static_cast<FLOAT>(i) * _perlinScale, static_cast<FLOAT>(j) * _perlinScale);
			//FLOAT val2 = val * range;
			//FLOAT val3 = val2 + _minHeight;
			_heightMap[i][j] = val * range + _minHeight;
		}
	}
}

// Calculate perlin noise for an x-z coordinate pair
FLOAT HeightMap::calcPerlinNoise(FLOAT x, FLOAT z)
{
	// Setup noise values
	FLOAT noise = 0;
	FLOAT persistence = 0.5f;
	UINT numOctaves = 4;
	UINT octaves = numOctaves - 1;
	
	// Loop through each octave to create noise
	for(UINT i = 0; i < octaves; ++i)
	{
		// Calculate current frequency and persistence of the noise
		FLOAT frequency = static_cast<FLOAT>(2 << i);
		FLOAT amplitude = powf(persistence, static_cast<FLOAT>(i));

		// Find the noise value for the current octave
		FLOAT interpolatedNoise = calcRandomNoise(x * frequency, z * frequency);//calcInterpolatedPerlinNoise(x * frequency, z * frequency);

		// Scale by amplitude and add to the total noise
		noise += interpolatedNoise * amplitude;
	}

	return noise;
}

// Calculate interpolated value for perlin noise
FLOAT HeightMap::calcInterpolatedPerlinNoise(FLOAT x, FLOAT z)
{
	// Find fractional remainder and floor of both x and z
	FLOAT floorX = floorf(x);
	FLOAT fracX = x - floorX;

	FLOAT floorZ = floorf(z);
	FLOAT fracZ = z - floorZ;

	// Find the smoothed noise for the surrounding points
	FLOAT v1 = calcSmoothedPerlinNoise(floorX, floorZ);
	FLOAT v2 = calcSmoothedPerlinNoise(floorX + 1.0f, floorZ);
	FLOAT v3 = calcSmoothedPerlinNoise(floorX, floorZ + 1.0f);
	FLOAT v4 = calcSmoothedPerlinNoise(floorX + 1.0f, floorZ + 1.0f);

	FLOAT i1 = calcInterpolation(v1, v2, fracX);
	FLOAT i2 = calcInterpolation(v3, v4, fracX);

	return calcInterpolation(i1, i2, fracZ);
}

// Calculate smoothed noise for a given value
FLOAT HeightMap::calcSmoothedPerlinNoise(FLOAT x, FLOAT z)
{
	// Create spans for x and z
	FLOAT xNeg = x - 1;
	FLOAT xPos = x + 1;

	FLOAT zNeg = z - 1;
	FLOAT zPos = z + 1;	

	// Calculate corner values
	FLOAT corners =	(calcRandomNoise(xNeg, zNeg) + 
					calcRandomNoise(xPos, zNeg) + 
					calcRandomNoise(xNeg, zPos) + 
					calcRandomNoise(xPos, zPos)) / 
					16.0f;
	FLOAT sides =	(calcRandomNoise(xNeg, z) + 
					calcRandomNoise(xPos, z) + 
					calcRandomNoise(x, zNeg) + 
					calcRandomNoise(x, zPos)) /
					8.0f;
	FLOAT center = calcRandomNoise(x, z) / 4.0f;
	return corners + sides + center;
}

// Interpolate between two values
FLOAT HeightMap::calcInterpolation(FLOAT a, FLOAT b, FLOAT t)
{
	FLOAT ft = t * PI;
	FLOAT f = (1.0f - std::cosf(ft)) * 0.5f;

	return (a * (1.0f - f)) + (b * f);
}

// Create random noise
FLOAT HeightMap::calcRandomNoise(FLOAT x, FLOAT z)
{
	// Find the unit square that contains the point
	FLOAT floorX = floorf(x);
	FLOAT floorZ = floorf(z);

	UINT X = static_cast<INT>(floorX) & 255;
	UINT Z = static_cast<INT>(floorZ) & 255;
	
	// Find the relative x and z of the point in the square
	x -= floorX;
	z -= floorZ;

	// Compute fade curves for each x and z
	FLOAT fadeX = calcFadeCurve(x);
	FLOAT fadeZ = calcFadeCurve(z);

	// Find hash values of the 4 corners of the square
	UINT A = (NOISE_PERMUTATION[X] + Z) % 256;
	UINT AA = (NOISE_PERMUTATION[A] + Z) % 256;
	UINT AB = (NOISE_PERMUTATION[A + 1] + Z) % 256;
	UINT B = (NOISE_PERMUTATION[X + 1] + Z) % 256;
	UINT BA = (NOISE_PERMUTATION[B] + Z) % 256;
	UINT BB = (NOISE_PERMUTATION[B + 1] + Z) % 256;

	// Add blended results from 4 corners of the square
	// Calculate gradients
	FLOAT gradAA = calcGradient(NOISE_PERMUTATION[AA], x, z);
	FLOAT gradBA = calcGradient(NOISE_PERMUTATION[BA], x - 1, z);
	FLOAT gradAB = calcGradient(NOISE_PERMUTATION[AB], x, z - 1);
	FLOAT gradBB = calcGradient(NOISE_PERMUTATION[BB], x - 1, z -1);

	// Lerp AA BA
	FLOAT lerpAABA = calcInterpolation(fadeX, gradAA, gradBA);
	
	// Lerp AB BB
	FLOAT lerpABBB = calcInterpolation(fadeX, gradAB, gradBB);

	// Final interpolation
	return calcInterpolation(fadeZ, lerpAABA, lerpABBB);
}

// Calculate the fade curve
FLOAT HeightMap::calcFadeCurve(FLOAT t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

// Calculate the gradient
FLOAT HeightMap::calcGradient(INT hash, FLOAT x, FLOAT z)
{
	// Convert the low 4 bits of the hash code into a gradient
	INT h = hash & 15;

	float u = h < 8 ? x : z;
	float v = h < 4 ? z : (h == 12 || h == 14) ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// Seed the perlin noise permutation
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