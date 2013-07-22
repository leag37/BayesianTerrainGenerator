/**************************************************************************************************
* Title: SmurfsMatrix.cpp
* Authors: Gael Huber, Erin Mooney
* Description: Defines a matrix class of arbitrary size
**************************************************************************************************/
#include "SmurfsMatrix.h"
#include "SmurfsQuaternion.h"

/**************************************************************************************************
* Default constructor
**************************************************************************************************/
Matrix::Matrix(void) {
	rows = 0;
	columns = 0;
	matrix = 0;
}

/**************************************************************************************************
* Constructor defining matrix size
*
* int r Number of rows
* int c Number of columns
**************************************************************************************************/
Matrix::Matrix(unsigned int r, unsigned int c) {
	// Construct the body for a matrix
	matrix = 0;
	matrix = new float*[r];
	for(unsigned int i = 0; i < r; i++) {
		matrix[i] = new float[c];
	}

	// Initialize matrix
	for(unsigned int i = 0; i < r; i++) {
		for(unsigned int j = 0; j < c; j++) {
			matrix[i][j] = 0;
		}
	}

	// Save dimensions
	rows = r;
	columns = c;
}

/**************************************************************************************************
* Copy constructor
* 
* Matrix m		Matrix to be copied
**************************************************************************************************/
Matrix::Matrix(const Matrix& m){	
	// Store dimension information
	rows = m.rows;
	columns = m.columns;
	
	// Create matrix
	matrix = (float**) calloc(rows, sizeof(float*));
	for(unsigned int i = 0; i < rows; i++) {
		matrix[i] = (float*) calloc(columns, sizeof(float));
	}

	// Initialize matrix
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			float a = m.matrix[i][j];
			matrix[i][j] = m.matrix[i][j];
		}
	}
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
Matrix::~Matrix(void) {
	// Free all memory used for the matrix
	if(matrix != 0) {
		for(unsigned int i = 0; i < rows; i++) {
		
			free(matrix[i]);
		}
		free(matrix);
	}
}

/**************************************************************************************************
* Set this matrix to its transpose
**************************************************************************************************/
void Matrix::transpose(void) {
	// Create the base for our transpose matrix
	Matrix mat = Matrix(columns, rows);

	// Create the tranpose
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			mat.matrix[j][i] = matrix[i][j];
		}
	}

	// Set this equal to the transpose
	*this = mat;
}

/**************************************************************************************************
* Return the transpose to this matrix
**************************************************************************************************/
Matrix Matrix::getTranspose(void) {
	// Create the base for our transpose matrix
	Matrix* mat = new Matrix(columns, rows);

	// Create the tranpose
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			mat->matrix[j][i] = matrix[i][j];
		}
	}

	// Return the transpose
	return *mat;
}

/**************************************************************************************************
* Set this matrix to its inverse
**************************************************************************************************/
void Matrix::inverse(void) {
	// TODO
}

/**************************************************************************************************
* Return the inverse to this matrix
**************************************************************************************************/
Matrix Matrix::getInverse(void) {
	// TODO
	return Matrix();
}

/**************************************************************************************************
* Return the determinant for this matrix
**************************************************************************************************/
Matrix Matrix::getDeterminant(void) {
	// TODO
	return Matrix();
}


/**************************************************************************************************
* Returns a quaternion from a 3x3 matrix
**************************************************************************************************/
Quaternion Matrix::toQuaternion(void) {
	// Only return a valid quaternion if the matrix is a 3x3
	if(rows != 3 || columns != 3)
		return Quaternion();
	
	float qw, qx, qy, qz;	// Declare quaternion components

	// Calculate w
	qw = sqrt(1.0f + matrix[0][0] + matrix[1][1] + matrix[2][2]) / 2.0f;

	// Prevent division by zero
	if(qw == 0.0f) {
		qx = 0.0f;
		qy = 0.0f;
		qz = 0.0f;
	} else {
		// Calcualte x, y, z
		qx = (matrix[2][1] - matrix[1][2]) / (qw * 4.0f);
		qy = (matrix[0][2] - matrix[2][0]) / (qw * 4.0f);
		qz = (matrix[1][0] - matrix[0][1]) / (qw * 4.0f);
	}

	// Define the quaternion
	Quaternion* q = new Quaternion(qw, qx, qy, qz);
	return *q;
}

/**************************************************************************************************
* Overload operators
**************************************************************************************************/
void Matrix::operator=(const Matrix &m) {
	// Store dimension information
	rows = m.rows;
	columns = m.columns;
	
	// Create matrix
	matrix = (float**) calloc(rows, sizeof(float*));
	for(unsigned int i = 0; i < rows; i++) {
		matrix[i] = (float*) calloc(columns, sizeof(float));
	}

	// Initialize matrix
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			float a = m.matrix[i][j];
			matrix[i][j] = m.matrix[i][j];
		}
	}
}


Matrix Matrix::operator+(const Matrix &m) {
	// Check size compatibility
	if(m.rows != rows || m.columns != columns)
		return Matrix();

	// Create a new matrix
	Matrix* mat = new Matrix(rows, columns);

	// Add matrix
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			mat->matrix[i][j] = matrix[i][j] + m.matrix[i][j];
		}
	}

	return *mat;
}

Matrix Matrix::operator-(const Matrix &m) {
	// Check size compatibility
	if(m.rows != rows || m.columns != columns)
		return Matrix();

	// Create a new matrix
	Matrix* mat = new Matrix(rows, columns);

	// Subtract matrix
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			mat->matrix[i][j] = matrix[i][j] - m.matrix[i][j];
		}
	}

	return *mat;
}

Matrix Matrix::operator*(const Matrix &m) {
	// Check size compatibility
	if(columns != m.rows)
		return Matrix();

	// Create a new matrix
	Matrix* mat = new Matrix(rows, m.columns);

	// Multiply matrix
	for(unsigned int r = 0; r < mat->rows; r++) {
		for(unsigned int c = 0; c < mat->columns; c++) {
			// Value for current position in new matrix
			float v = 0.0f;
			
			// Multiply row r of M1 by column c of M2
			for(unsigned int i = 0; i < columns; i++) {
				v += matrix[r][i] * m.matrix[i][c];
			}

			// Assign the value to the matrix
			mat->matrix[r][c] = v;
		}
	}

	return *mat;
}

/**************************************************************************************************
* Multiplication
**************************************************************************************************/
void Matrix::operator*=(float multiplier) {
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			matrix[i][j] *= multiplier;
		}
	}
}

/**************************************************************************************************
* Division
**************************************************************************************************/
void Matrix::operator/=(float divisor) {
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			matrix[i][j] /= divisor;
		}
	}
}

bool Matrix::operator==(const Matrix &m) {
	// Check size compatibility
	if(m.rows != rows || m.columns != columns)
		return false;

	// Check equality, return false if ever not equal
	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < columns; j++) {
			if(matrix[i][j] != m.matrix[i][j])
				return false;
		}
	}

	// Success! Return true
	return true;
}