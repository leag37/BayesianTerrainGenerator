/**************************************************************************************************
* Title: SmurfsVector2.h
* Authors: Gael Huber
* Description: 2-dimensional vector and associated operations
**************************************************************************************************/
#ifndef __SMURFSVECTOR2_H__
#define __SMURFSVECTOR2_H__

#include <cmath>
#include <math.h>

#ifndef PI
#define PI 3.1415927f
#endif // PI

class Vector2 {
public:
	Vector2(void);	// Constructor
	Vector2(float x, float y);	// Constructor initializing the vector
	~Vector2(void);	// Destructor

	float magnitude(void);	// Returns the magnitude of the vector
	Vector2 getNormalized(void);	// Returns a normalized version of the vector without altering the
									// original vector.
	void normalize(void);	// Normalizes the vector.
	Vector2 normalVector(void);	// Return the normal to this vector

	float dot(Vector2 v);	// Dot product with another vector
	Vector2 floor();		// Return a vector composed of the floor of this function
	Vector2 mod(int inMod);	// Mod this vector by a given value

	// Overload operators
	Vector2 operator*(const float &);
	Vector2 operator+(const Vector2 &);
	Vector2 operator-(const Vector2 &);
	Vector2 operator/(const float &);
	void operator*=(const float &);
	void operator+=(const Vector2 &);
	void operator-=(const Vector2 &);
	void operator/=(const float &);

	float x,y;	// Vector components
};

extern Vector2 Vec2CosineInterpolation(const Vector2& a, const Vector2& b, float t);
extern Vector2 Vec2Scale(const Vector2& v, float s);

#endif