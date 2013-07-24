//
// Title: SmurfsMathUtils.h
// Author: Gael Huber
// Description: Utility function for math
//
#ifndef __SMURFSMATHUTILS_H__
#define __SMURFSMATHUTILS_H__

#include <Windows.h>
#include <cmath>

FLOAT clampf(FLOAT a, FLOAT min, FLOAT max);

INT clamp(INT a, INT min, INT max);

UINT clampu(UINT a, UINT min, UINT max);

FLOAT randf(FLOAT min, FLOAT max);

#endif // __SMURFSMATHUTILS_H__
