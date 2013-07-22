/**************************************************************************************************
* Title: SmurfsMatrix.h
* Authors: Gael Huber, Erin Mooney
* Description: Defines a matrix class of arbitrary size
**************************************************************************************************/
#ifndef __SMURFSMATRIX_H__
#define __SMURFSMATRIX_H__

#include <math.h>
#include <stdlib.h>

class Quaternion;

class Matrix {
public:
	Matrix(void);	// Default constructor
	Matrix(unsigned int r, unsigned int c);	// Constructor defining matrix size
	Matrix(const Matrix& m);	// Copy constructor
	~Matrix(void);	// Destructor

	void transpose(void);	// Set this matrix to its transpose
	Matrix getTranspose(void);	// Return the transpose to this matrix
	void inverse(void);		// Set this matrix to its inverse
	Matrix getInverse(void);	// Return the inverse to this matrix
	Matrix getDeterminant(void);	// Return the determinant for this matrix
	Quaternion toQuaternion(void);	// Returns a quaternion from a 3x3 matrix

	// Overload operators
	void operator=(const Matrix &);
	Matrix operator+(const Matrix &);
	Matrix operator-(const Matrix &);
	Matrix operator*(const Matrix &);
	
	void operator*=(float multiplier);
	void operator/=(float divisor);

	bool operator==(const Matrix &);

	unsigned int rows;	// Number of rows
	unsigned int columns;	// Number of columns
	float** matrix;	// Matrix of numbers
};

#endif