/**************************************************************************************************
* Title: SmurfsVector3.h
* Authors: Gael Huber
* Description: 3-dimensional vector and associated operations
**************************************************************************************************/
#ifndef __SMURFSVECTOR3_H__
#define __SMURFSVECTOR3_H__

#include <math.h>
#include <cmath>

class Quaternion;

class Vector3 {
public:
	Vector3(void);		// default constructor
	Vector3(float x, float y, float z);		// constructor initializing vector
	Vector3(const Vector3& v);		// copy constructor =)
	~Vector3(void);		// destructor

	float magnitude(void);	// Returns the magnitude of the vector
	Vector3 getNormalized(void);	// Returns a normalized version of the vector without altering the
									// original vector.
	void normalize(void);	// Normalizes the vector.
	Vector3 normalVector(void);	// Return the normal to this vector

	Vector3 cross(Vector3 v);	// Cross product with another vector
	float dot(Vector3 v);	// Dot product with another vector

	Vector3 floor();	// Conduct a floor operation on all attributes of the vector

	void rotate(Quaternion q);	// Rotate the vector by a quaternion

	// Overload operators
	Vector3 operator*(const float &);
	Vector3 operator+(const Vector3 &);
	Vector3 operator-(const Vector3 &);
	Vector3 operator/(const float &);
	float operator[](const int &);
	void operator*=(const float &);
	void operator+=(const Vector3 &);
	void operator-=(const Vector3 &);
	void operator/=(const float &);
	bool operator ==(const Vector3& v) const;	// comparison of vectors
	bool operator !=(const Vector3& v) const;

	float x,y,z;	// vector components
};

#endif