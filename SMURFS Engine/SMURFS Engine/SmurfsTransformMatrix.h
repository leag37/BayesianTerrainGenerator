/**************************************************************************************************
* Title: SmurfsTransformMatrix.h
* Author: Gael Huber
* Desription: Class that describes a mesh's transform matrix. Can be used for perspective, view, 
*	and model matrices. This defines a column-based matrix, NOT a row-based matrix.
**************************************************************************************************/
#ifndef __SMURFSTRANSFORMMATRIX_H__
#define __SMURFSTRANSFORMMATRIX_H__

class TransformMatrix {
public:
	TransformMatrix(void);	// Default constructor
	~TransformMatrix(void);	// Destructor

	void transpose(void);	// Set this matrix to its transpose
	Matrix getTranspose(void);	// Return the transpose to this matrix
	void inverse(void);		// Set this matrix to its inverse
	Matrix getInverse(void);	// Return the inverse to this matrix
	Matrix getDeterminant(void);	// Return the determinant for this matrix

	// Overload operators
	void operator=(const Matrix &);
	Matrix operator+(const Matrix &);
	Matrix operator-(const Matrix &);
	Matrix operator*(const Matrix &);
	bool operator==(const Matrix &);

	unsigned int rows;	// Number of rows
	unsigned int columns;	// Number of columns
	float** matrix;	// Matrix of numbers
};

#endif