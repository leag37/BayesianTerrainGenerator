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
HeightMap::HeightMap(
	FLOAT inXWidth, FLOAT inZWidth, 
	FLOAT inMinHeight, FLOAT inMaxHeight,
	FLOAT inStartX, FLOAT inStartZ,
	UINT inResolution)
	:	_heightMap(0), _flowMap(0), 
		_xWidth(inXWidth), _zWidth(inZWidth),
		_minHeight(inMinHeight), _maxHeight(inMaxHeight),
		_startX(inStartX), _startZ(inStartZ),
		_resolution(inResolution)
{ 
	// Initialize the height and flow map
	_flowMap = new Vector3*[_resolution];
	_heightMap = new FLOAT*[_resolution];
	for(UINT i = 0; i < _resolution; ++i)
	{
		_flowMap[i] = new Vector3[_resolution];
		_heightMap[i] = new FLOAT[_resolution];

		for(UINT j = 0; j < _resolution; ++j)
		{
			_flowMap[i][j] = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
			_heightMap[i][j] = FLT_MAX;
		}
	}

	// Set perlin scale
	_perlinScale = 1.0f;//0.01f;
}

HeightMap::~HeightMap()
{

}

// Height map height data
FLOAT** HeightMap::heightMap()
{
	return _heightMap;
}

// Flow map data
Vector3** HeightMap::flowMap()
{
	return _flowMap;
}

// Heightmap properties
UINT HeightMap::resolution()
{
	return _resolution;
}

FLOAT HeightMap::minHeight()
{
	return _minHeight;
}

FLOAT HeightMap::maxHeight()
{
	return _maxHeight;
}

// Generate the height map
void HeightMap::generate()
{
	// Calculate range
	FLOAT range = (_maxHeight - _minHeight) * 0.5f;

	// Seed the flow map
	seedFlowMap();

	// Iterate through the height map and generate noise for each coordinate
	for(UINT i = 0; i < _resolution; ++i)
	{
		for(UINT j = 0; j < _resolution; ++j)
		{
			FLOAT val = calcPerlinNoise(static_cast<FLOAT>(i) * _perlinScale, static_cast<FLOAT>(j) * _perlinScale, 0.0f, 50);

			//FLOAT dist = plotDistribution(val);

			val /= 0.87f;	// Expected normal range of the noise function
			FLOAT height = (val + 1.0f) * range + _minHeight;
			_heightMap[i][j] = height;
		}
	}
}

// Seed the flow map with random unit vectors
void HeightMap::seedFlowMap()
{
	// Convert max random number into a float
	FLOAT fRandMax = static_cast<FLOAT>(RAND_MAX) * 0.5f;

	// Iterate through each possible value of the flow map and assign a random unit vector
	for(UINT i = 0; i < _resolution; ++i)
	{
		for(UINT j = 0; j < _resolution; ++j)
		{
			// Check whether we need to initialize this value
			if(_flowMap[i][j] == Vector3(FLT_MAX, FLT_MAX, FLT_MAX))
			{
				// Generate flow values
				FLOAT x = (static_cast<FLOAT>(rand()) / fRandMax) - 1.0f;
				FLOAT y = (static_cast<FLOAT>(rand()) / fRandMax) - 1.0f;
				FLOAT z = (static_cast<FLOAT>(rand()) / fRandMax) - 1.0f;

				// Set into a vector
				Vector3 v = Vector3(x, y, z);

				// Normalize
				v.normalize();

				// Set as the flow vector for this index
				_flowMap[i][j] = v;
			}
		}
	}
}

// Calculate perlin noise for an x-z coordinate pair
FLOAT HeightMap::calcPerlinNoise(FLOAT x, FLOAT y, FLOAT z, UINT numSamples)
{
	// Setup noise values
	FLOAT noise = 0;
	FLOAT persistence = 0.5f;
	UINT numOctaves = 4;
	
	// Loop through each octave to create noise
	for(UINT i = 0; i < numOctaves; ++i)
	{
		// Calculate current frequency and persistence of the noise
		FLOAT frequency = powf(2.0f, static_cast<FLOAT>(i));
		FLOAT amplitude = powf(persistence, static_cast<FLOAT>(i));

		// Find the noise value for the current octave with n samples
		FLOAT interpolatedNoise = 0.0f;
		for(UINT j = 0; j < numSamples; ++j)
		{
			// Create a sample x and y value
			FLOAT sx = randf(0.0f, 1.0f);
			FLOAT sy = randf(0.0f, 1.0f);
			FLOAT sz = 0.0f;//randf(0.0f, 1.0f);

			// Set the random point
			sx += x;
			sy += y;
			sz += z;

			FLOAT xFrequency = frequency / static_cast<FLOAT>(_resolution);
			FLOAT yFrequency = frequency / static_cast<FLOAT>(_resolution);
			FLOAT tNoise = calcRandomNoise(sx * xFrequency, sy * xFrequency, sz * frequency);//calcInterpolatedPerlinNoise(x * frequency, z * frequency);
			interpolatedNoise += tNoise;
		}
		interpolatedNoise /= static_cast<FLOAT>(numSamples);

		// Scale by amplitude and add to the total noise
		noise += interpolatedNoise * amplitude;
	}

	return noise;
}

// Interpolate between two values
FLOAT HeightMap::calcInterpolation(FLOAT a, FLOAT b, FLOAT t)
{
	FLOAT ft = t * PI;
	FLOAT f = (1.0f - std::cosf(ft)) * 0.5f;

	return (a * (1.0f - f)) + (b * f);
}

// Create random noise
FLOAT HeightMap::calcRandomNoise(FLOAT x, FLOAT y, FLOAT z)
{
	// Find the unit square that contains the point
	FLOAT floorX = floorf(x);
	FLOAT floorY = floorf(y);
	FLOAT floorZ = floorf(z);

	UINT X = static_cast<INT>(floorX) & 255;
	UINT Y = static_cast<INT>(floorY) & 255;
	UINT Z = static_cast<INT>(floorZ) & 255;

	UINT xx = static_cast<INT>(x) % _resolution;
	UINT yy = static_cast<INT>(y) % _resolution;
	UINT zz = static_cast<INT>(z);

	UINT xx1 = clampu(xx + 1, 0, _resolution - 1);
	UINT yy1 = clampu(yy + 1, 0, _resolution - 1);
	UINT zz1 = zz;
	
	// Find the relative x and z of the point in the square
	x -= floorX;
	y -= floorY;
	z -= floorZ;

	// Compute fade curves for each x and z
	FLOAT fadeX = calcFadeCurve(x);
	FLOAT fadeY = calcFadeCurve(y);
	FLOAT fadeZ = calcFadeCurve(z);

	// Find hash values of the 4 corners of the square
	UINT A = (NOISE_PERMUTATION[X] + Y);
	UINT AA = (NOISE_PERMUTATION[A] + Z);
	UINT AB = (NOISE_PERMUTATION[A + 1] + Z);
	UINT B = (NOISE_PERMUTATION[X + 1] + Y);
	UINT BA = (NOISE_PERMUTATION[B] + Z);
	UINT BB = (NOISE_PERMUTATION[B + 1] + Z);

	// Add blended results from 4 corners of the square
	// Calculate gradients for closest value
	FLOAT grad1 = calcGrad(NOISE_PERMUTATION[AA], xx, yy, zz);
	FLOAT grad2 = calcGrad(NOISE_PERMUTATION[BA], xx1, yy, zz);
	FLOAT grad3 = calcGrad(NOISE_PERMUTATION[AB], xx, yy1, zz);
	FLOAT grad4 = calcGrad(NOISE_PERMUTATION[BB], xx1, yy1, zz);

	// Lerp between values
	FLOAT lerp1 = calcInterpolation(grad1, grad2, fadeX);
	FLOAT lerp2 = calcInterpolation(grad3, grad4, fadeX);
	FLOAT lerp3 = calcInterpolation(lerp1, lerp2, fadeY);
	return lerp3;


	FLOAT gradAA = calcGradient(NOISE_PERMUTATION[AA], x, y, z);
	FLOAT gradBA = calcGradient(NOISE_PERMUTATION[BA], x - 1, y, z);
	FLOAT gradAB = calcGradient(NOISE_PERMUTATION[AB], x, y - 1, z);
	FLOAT gradBB = calcGradient(NOISE_PERMUTATION[BB], x - 1, y - 1, z);
	FLOAT gradAA1 = calcGradient(NOISE_PERMUTATION[AA + 1], x, y, z - 1);
	FLOAT gradBA1 = calcGradient(NOISE_PERMUTATION[BA + 1], x - 1, y, z + 1);
	FLOAT gradAB1 = calcGradient(NOISE_PERMUTATION[AB + 1], x, y - 1, z - 1);
	FLOAT gradBB1 = calcGradient(NOISE_PERMUTATION[BB + 1], x - 1, y - 1, z - 1);

	// Calculate lerps
	FLOAT lerpAABA = calcInterpolation(gradAA, gradBA, fadeX);
	FLOAT lerpABBB = calcInterpolation(gradAB, gradBB, fadeX);
	FLOAT lerpAABA1 = calcInterpolation(gradAA1, gradBA1, fadeX);
	FLOAT lerpABBB1 = calcInterpolation(gradAB1, gradBA1, fadeX);

	FLOAT cmbLerp1 = calcInterpolation(lerpAABA, lerpABBB, fadeY);
	FLOAT cmbLerp2 = calcInterpolation(lerpAABA1, lerpABBB1, fadeY);
	return calcInterpolation(cmbLerp1, cmbLerp2, fadeZ);
}

// Calculate the fade curve
FLOAT HeightMap::calcFadeCurve(FLOAT t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

// Calculate the gradient
FLOAT HeightMap::calcGradient(INT hash, FLOAT x, FLOAT y, FLOAT z)
{
	// Convert the low 4 bits of the hash code into a gradient
	INT h = hash & 15;

	float u = h < 8 ? x : y;
	float v = h < 4 ? y : (h == 12 || h == 14) ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// Calculate the gradient based on integer values
FLOAT HeightMap::calcGrad(INT hash, UINT x, UINT y, UINT z)
{
	// Grab the flow map gradient
	Vector3 grad = _flowMap[x][y];

	// Hash
	FLOAT val = calcGradient(hash, grad.x, grad.y, grad.z);
	return val;
}

// Prefill the flowmap with other height maps
void HeightMap::prefillFlowMap(HeightMap* hm)
{
	// Find the side of interest for prefill
	//			x x x
	//			x x x
	//			x x x
	//	  o o o	- - -
	//	  o o o	- - - 
	//	  o o o	- - - 
	// startX(o) = 1, startZ(o) = 4
	// startX(-) = 4, startZ(-) = 4
	// startX(x) = 4, startZ(x) = 1
	Vector3** fm = hm->flowMap();
	if(_startX == hm->startX())
	{
		// Prefill top side, so we must make the first row of this chunk equivalent to the last row of the previous chunk
		for(UINT i = 0; i < _resolution; ++i)
		{
			_flowMap[0][i] = fm[_resolution - 1][i];
		}
	}
	else if(_startZ == hm->startZ())
	{
		// Prefil left side so we must make the left-most column of this chunk equivalent to the right-most column of the previous chunk
		for(UINT i = 0; i < _resolution; ++i)
		{
			_flowMap[i][0] = fm[i][_resolution - 1];
		}
	}
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
			NOISE_PERMUTATION[index] = NOISE_PERMUTATION[index + 256] = i;
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
					NOISE_PERMUTATION[index] = NOISE_PERMUTATION[index + 256] = i;
					seekInsertionPoint = false;
				}
			}
		}
	}
}