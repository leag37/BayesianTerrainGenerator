/**************************************************************************************************
* Title: SmurfsMatrixReader.h
* Authors: Gael Huber, Erin Mooney
* Description: Reads in a matrix from a specified file.
**************************************************************************************************/
#ifndef __SMURFSMATRIXREADER_H__
#define __SMURFSMATRIXREADER_H__

#include <fstream>
#include <string>
#include <vector>
#include "SmurfsMatrix.h"
#include "SmurfsSingleton.h"

using namespace std;

class MatrixReader : public Singleton<MatrixReader> {
public:
	MatrixReader(void);	// Constructor
	~MatrixReader(void);	// Destructor

	static MatrixReader& getSingleton(void);	// Get the singleton instance
	static MatrixReader* getSingletonPtr(void);	// Get the singleton pointer

	Matrix loadMatrix(const char* file);	// reads in a matrix from a file

private:
	void tokenizeString(string str, vector<string>& tokens, char delimiter);	// Tokenize a string a return a vector of tokens
	string baseDirectory;
};

#endif