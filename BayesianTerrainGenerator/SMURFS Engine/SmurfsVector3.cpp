/**************************************************************************************************
* Title: SmurfsVector3.cpp
* Authors: Gael Huber
* Description: 3-dimensional vector and associated operations
**************************************************************************************************/
#include "SmurfsVector3.h"
#include "SmurfsQuaternion.h"

/**************************************************************************************************
* Constructor
**************************************************************************************************/
Vector3::Vector3(void) {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

/**************************************************************************************************
* Constructor
*
* float x		x value
* float y		y value
**************************************************************************************************/
Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

/**************************************************************************************************
* Copy constructor
* 
* Vector3 v		Vector to be copied
**************************************************************************************************/
Vector3::Vector3(const Vector3& v){
	x = v.x;
	y = v.y;
	z = v.z;
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
Vector3::~Vector3(void) {
	// Do nothing
}

/**************************************************************************************************
* Returns the magnitude of the vector
**************************************************************************************************/
float Vector3::magnitude(void) {
	return sqrt(x*x + y*y + z*z);
}

/**************************************************************************************************
* Returns a normalized version of the vector without altering the original vector
**************************************************************************************************/
Vector3 Vector3::getNormalized(void) {
	float mag = magnitude();	// What is the current magnitude?

	// If it is a zero vector, return a zero vector
	if(mag == 0.0f)
		return Vector3();

	return Vector3(x/mag, y/mag, z/mag);
}

/**************************************************************************************************
* Normalizes the vector
**************************************************************************************************/
void Vector3::normalize(void) {
	float mag = magnitude();	// What is the current magnitude?

	// If it is not a zero vector, normalize
	if(mag != 0.0f) {
		x /= mag;
		y /= mag;
		z /= mag;
	}
}

/**************************************************************************************************
* Return the normal to this vector
**************************************************************************************************/
Vector3 Vector3::normalVector(void) {
	return Vector3();
}

/**************************************************************************************************
* Cross product with another vector
**************************************************************************************************/
Vector3 Vector3::cross(Vector3 v) {
	 float a = y * v.z - z * v.y;
     float b = z * v.x - x * v.z;
     float c = x * v.y - y * v.x;
	 return Vector3(a,b,c);
}

/**************************************************************************************************
* Rotate the vector by a quaternion
**************************************************************************************************/
void Vector3::rotate(Quaternion q) {
	// Define the vector to rotate as a quaternion
	Quaternion vectorQuaternion = Quaternion(0.0f, x, y, z);

	// Multiply the quaternions to apply a rotation
	Quaternion v = q * vectorQuaternion * q.getConjugate();

	// Update the vector by extracting the vector from the rotated quaternion
	x = v.x;
	y = v.y;
	z = v.z;
}

/**************************************************************************************************
* Dot product with another vector
**************************************************************************************************/
float Vector3::dot(Vector3 v) {
	return x * v.x + y * v.y + z * v.z;
}

/**************************************************************************************************
* Multiplication
**************************************************************************************************/
Vector3 Vector3::operator*(const float &v) {
	return Vector3(x * v, y * v, z * v);
}

/**************************************************************************************************
* Addition
**************************************************************************************************/
Vector3 Vector3::operator+(const Vector3 &v) {
	return Vector3(x + v.x, y + v.y, z + v.z);
}

/**************************************************************************************************
* Subtraction
**************************************************************************************************/
Vector3 Vector3::operator-(const Vector3 &v) {
	return Vector3(x - v.x, y - v.y, z - v.z);
}

/**************************************************************************************************
* Division
**************************************************************************************************/
Vector3 Vector3::operator/(const float &v) {
	return Vector3(x / v, y / v, z / v);
}

/**************************************************************************************************
* Array-based accessor
**************************************************************************************************/
float Vector3::operator[](const int &i) {
	switch(i) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}
	return 0.0f;
}

/**************************************************************************************************
* Multiplication (dot product)
**************************************************************************************************/
void Vector3::operator*=(const float &v) {
	x *= v;
	y *= v;
	z *= v;
}

/**************************************************************************************************
* Addition
**************************************************************************************************/
void Vector3::operator+=(const Vector3 &v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

/**************************************************************************************************
* Subtraction
**************************************************************************************************/
void Vector3::operator-=(const Vector3 &v) {
	x -= v.x;
	y -= v.y;
	y -= v.z;
}

/**************************************************************************************************
* Division
**************************************************************************************************/
void Vector3::operator/=(const float &v) {
	x /= v;
	y /= v;
	z /= v;
}

/**************************************************************************************************
* Test two vectors for equality
**************************************************************************************************/
bool Vector3::operator ==(const Vector3& v) const{
    return x==v.x && y==v.y && z==v.z;
}
        
/**************************************************************************************************
* Test two vectors for inequality
**************************************************************************************************/
bool Vector3::operator !=(const Vector3& v) const{
    return x!=v.x || y!=v.y || z!=v.z;
}