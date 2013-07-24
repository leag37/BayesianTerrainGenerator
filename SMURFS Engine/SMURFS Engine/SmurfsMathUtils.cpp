#include "SmurfsMathUtils.h"

FLOAT clampf(FLOAT a, FLOAT min, FLOAT max)
{
	if(a < min)
	{
		a = min;
	}
	else if(a > max)
	{
		a = max;
	}
	return a;
}

INT clamp(INT a, INT min, INT max)
{
	if(a < min)
	{
		a = min;
	}
	else if(a > max)
	{
		a = max;
	}
	return a;
}

UINT clampu(UINT a, UINT min, UINT max)
{
	if(a < min)
	{
		a = min;
	}
	else if(a > max)
	{
		a = max;
	}
	return a;
}

FLOAT randf(FLOAT min, FLOAT max)
{
	// Random value between 0 and 1
	FLOAT r = static_cast<FLOAT>(rand()) / static_cast<FLOAT>(RAND_MAX);

	return r * (max - min) + min;
}


