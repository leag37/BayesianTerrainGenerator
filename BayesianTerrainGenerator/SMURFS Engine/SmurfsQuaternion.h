/**************************************************************************************************
* Title: Quaternion.h
* Authors: Gael Huber
* Description: Implementation of a quaternion class.
**************************************************************************************************/
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#define PI 3.14159265f

class Vector3;
class Matrix;

class Quaternion {
public:
	Quaternion(void);	// Constructor, defines an identity quaternion
	Quaternion(float w, float x, float y, float z);	// Constructor defining a quaternion
	Quaternion(float radians, Vector3 v);	// Construct a quaternion via an angle and a basis vector,
		// this will build a unit quaternion.
	~Quaternion(void);	// Destructor

	void inverse(void);	// Set this quaternion to its inverse
	Quaternion getInverse(void);	// Return the inverse of this quaternion
	void conjugate(void);	// Set this quaternion to its conjugate
	Quaternion getConjugate(void);	// Return the conjugate of this quaternion
	float magnitude(void);	// Return the magnitude of the quaternion
	Matrix toMatrix(void);	// Convert to and return a matrix representation of the quaternion

	// Overload operators
	Quaternion operator+(const Quaternion &);
	Quaternion operator-(const Quaternion &);
	Quaternion operator*(const Quaternion &);
	Quaternion operator*(const float &);
	Quaternion operator/(const Quaternion &);
	Quaternion operator/(const float &);

	void operator+=(const Quaternion &);
	void operator-=(const Quaternion &);
	void operator*=(const Quaternion &);
	void operator*=(const float &);
	void operator/=(const Quaternion &);
	void operator/=(const float &);

	float w, x, y, z;	// Variables defining a quaternion

	static const Quaternion IDENTITY;	// Identity quaternion
};

#endif