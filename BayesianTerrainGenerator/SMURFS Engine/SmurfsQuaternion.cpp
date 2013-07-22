/**************************************************************************************************
* Title: Quaternion.cpp
* Authors: Gael Huber
* Description: Implementation of a quaternion class.
**************************************************************************************************/
#include "SmurfsQuaternion.h"
#include "SmurfsVector3.h"
#include "SmurfsMatrix.h"	

// Identity quaternion
const Quaternion Quaternion::IDENTITY = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

/**************************************************************************************************
* Constructor, defines an identity quaternion
**************************************************************************************************/
Quaternion::Quaternion(void) {
	w = 1.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

/**************************************************************************************************
* Constructor, defines an identity quaternion
**************************************************************************************************/
Quaternion::Quaternion(float radians, Vector3 v) {
	// Find the sin and cosine values in radians
	float c = cos(radians / 2.0f);
	float s = sin(radians / 2.0f);
	
	// Normalize the vector to create a unit vector
	v.normalize();	

	// Create the unit quaternion
	w = c;
	x = v.x * s;
	y = v.y * s;
	z = v.z * s;
}

/**************************************************************************************************
* Constructor, defines an identity quaternion
**************************************************************************************************/
Quaternion::Quaternion(float w, float x, float y, float z) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quaternion::~Quaternion(void) {
}

/**************************************************************************************************
* Set this quaternion to its inverse
* q^-1 = q*
*       _____
*       |q|^2
**************************************************************************************************/
void Quaternion::inverse(void) {
	// Get the conjugate and the magnitude
	Quaternion q = getConjugate();
	float mag = magnitude();

	// Divide the conjugate by the magnitude squared
	q /= (mag * mag);

	// Update the internal values of the quaternion
	w = q.w; x = q.x; y = q.y; z = q.z;
}


/**************************************************************************************************
* Return the inverse of this quaternion
**************************************************************************************************/
Quaternion Quaternion::getInverse(void) {
	// Get the conjugate and the magnitude
	Quaternion q = getConjugate();
	float mag = magnitude();

	// Divide the conjugate by the magnitude squared
	q /= (mag * mag);

	return q;
}

/**************************************************************************************************
* Set this quaternion to its conjugate
* q* = [-qv qs]
**************************************************************************************************/
void Quaternion::conjugate(void) {
	x *= -1;
	y *= -1;
	z *= -1;
}

/**************************************************************************************************
* Return the conjugate of this quaternion
**************************************************************************************************/
Quaternion Quaternion::getConjugate(void) {
	return Quaternion(w, -x, -y, -z);
}

/**************************************************************************************************
* Return the magnitude of the quaternion
**************************************************************************************************/
float Quaternion::magnitude(void) {
	return sqrt(w*w + x*x + y*y + z*z);
}

/**************************************************************************************************
* Convert to and return a matrix representation of the quaternion
**************************************************************************************************/
Matrix Quaternion::toMatrix(void) {
	Matrix* m = new Matrix(3, 3);
	m->matrix[0][0] = 1 - 2*y*y - 2*z*z;
	m->matrix[0][1] = 2*x*y + 2*w*z;
	m->matrix[0][2] = 2*x*z - 2*w*y;

	m->matrix[1][0] = 2*x*y - 2*w*z;
	m->matrix[1][1] = 1 - 2*x*x - 2*z*z;
	m->matrix[1][2] = 2*y*z + 2*w*x;
	
	m->matrix[2][0] = 2*x*z + 2*w*y;
	m->matrix[2][1] = 2*y*z - 2*w*x;
	m->matrix[2][2] = 1 - 2*x*x - 2*y*y;

	return *m;
}

/**************************************************************************************************
* Return the addition of two quaternions.
**************************************************************************************************/
Quaternion Quaternion::operator+(const Quaternion &q) {
	return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

/**************************************************************************************************
* Return the subtraction of two quaternions.
**************************************************************************************************/
Quaternion Quaternion::operator-(const Quaternion &q) {
	return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}

/**************************************************************************************************
* Return the multiplication of two quaternions
**************************************************************************************************/
Quaternion Quaternion::operator*(const Quaternion &q) {
	Vector3 current = Vector3(x, y, z);	// Vector composed of the quaternion
	Vector3 param = Vector3(q.x, q.y, q.z);	// Vector composed of the parameter quaternion
	float f = w * q.w - current.dot(param);	// Find the scalar component for the new quaternion
	Vector3 v = param * w + current * q.w + current.cross(param);
	return Quaternion(f, v.x, v.y, v.z);
}

/**************************************************************************************************
* Return the multiplication of a quaternion and a float. This affects magnitude.
**************************************************************************************************/
Quaternion Quaternion::operator*(const float &f) {
	return Quaternion(w * f, x * f, y * f, z * f);
}

/**************************************************************************************************
* Return the division of two quaternions
**************************************************************************************************/
Quaternion Quaternion::operator/(const Quaternion &q) {
	Quaternion quat = q;
	return (*this * quat.getInverse());
}

/**************************************************************************************************
*  Return the division of a quaternion and a float. This affects magnitude.
**************************************************************************************************/
Quaternion Quaternion::operator/(const float &f) {
	return Quaternion(w / f, x / f, y / f, z / f);
}

/**************************************************************************************************
* Add a quaternion to the current quaternion
**************************************************************************************************/
void Quaternion::operator+=(const Quaternion &q) {
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
}

/**************************************************************************************************
* Subtract a quaternion from the current quaternion
**************************************************************************************************/
void Quaternion::operator-=(const Quaternion &q) {
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
}

/**************************************************************************************************
* Multiply the current quaternion by a quaternion
**************************************************************************************************/
void Quaternion::operator*=(const Quaternion &q) {
	Vector3 current = Vector3(x, y, z);	// Vector composed of the quaternion
	Vector3 param = Vector3(q.x, q.y, q.z);	// Vector composed of the parameter quaternion
	float f = w * q.w - current.dot(param);	// Find the scalar component for the new quaternion
	Vector3 v = param * w + current * q.w + current.cross(param);	// Multiply to find the vector
		// component of the quaternion.
	// Update the quaternion
	w = f;	x = v.x;	y = v.y;	z = v.z;
}

/**************************************************************************************************
* Multiply the current quaternion by a float. This affects magnitude.
**************************************************************************************************/
void Quaternion::operator*=(const float &f) {
	w *= f;
	x *= f;
	y *= f;
	z *= f;
}

/**************************************************************************************************
* Divide the quaternion by another quaternion. 
**************************************************************************************************/
void Quaternion::operator/=(const Quaternion &q) {
	Quaternion quat = q;
	*this *= quat.getInverse();
}

/**************************************************************************************************
* Divide the quaternion by a float. This affects magnitude.
**************************************************************************************************/
void Quaternion::operator/=(const float &f) {
	w /= f;
	x /= f;
	y /= f;
	z /= f;
}