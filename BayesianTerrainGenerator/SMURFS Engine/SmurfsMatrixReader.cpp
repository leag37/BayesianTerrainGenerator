/**************************************************************************************************
* Title: SmurfsMatrixReader.cpp
* Authors: Gael Huber, Erin Mooney
* Description: Reads in a matrix from a specified file.
**************************************************************************************************/
#include "SmurfsMatrixReader.h"

// Singleton instance
template<> MatrixReader* Singleton<MatrixReader>::singleton = 0;

/**************************************************************************************************
* Constructor
**************************************************************************************************/
MatrixReader::MatrixReader(void) {
	// Base directory
	baseDirectory = "..\\media\\matrix\\";
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
MatrixReader::~MatrixReader(void) { }

/**************************************************************************************************
* Get the singleton instance
**************************************************************************************************/
MatrixReader& MatrixReader::getSingleton(void) { 
	assert(singleton);
	return *singleton;
}

/**************************************************************************************************
* Get the singleton pointer
**************************************************************************************************/
MatrixReader* MatrixReader::getSingletonPtr(void) {
	return singleton;
}

/**************************************************************************************************
* Reads in a matrix from a file
**************************************************************************************************/
Matrix MatrixReader::loadMatrix(const char* file) {
	// set up a stream and open the file
	std::ifstream fileStream;
	string filepath = baseDirectory + file;
	fileStream.open(filepath.c_str());

	// if the file doesn't exist return an empty matrix
	if(!fileStream) {
		return Matrix();
	}

	std::string line;	// string to hold each line
	Matrix* mat;	// matrix being read in
	int rowNumber = 0;	// The current row number being processed

	// get the first line
	getline(fileStream, line);

	// while the file stream still exists, process the string
	while(fileStream) {
		// If the next string will be determining the dimensions of the matrix
		if(line == "size") {
			getline(fileStream, line);
			vector<string> tokens;
			tokenizeString(line, tokens, ' ');
			
			// Get rows and columns and initialize the matrix
			int rows = atoi(tokens[0].c_str());
			int columns = atoi(tokens[1].c_str());
			mat = new Matrix(rows, columns);
		}

		// If the next string will be a line in the matrix
		if(line == "matrix") {
			// Read in each row
			for(unsigned int i = 0; i < mat->rows; i++) {
				getline(fileStream, line);
				vector<string> tokens;
				tokenizeString(line, tokens, ' ');

				// Populate the matrix
				for(unsigned j = 0; j < mat->columns; j++) {
					mat->matrix[i][j] = (float)atof(tokens[j].c_str());
				}
			}
		}

		// get the next line
		getline(fileStream, line);
	}

	return *mat;
}

/**************************************************************************************************
* Tokenize a string a return a vector of tokens
**************************************************************************************************/
void MatrixReader::tokenizeString(string str, vector<string>& tokens, char delimiter) {

	// Skip all delimiters at the start of the string
	string::size_type lastPos = str.find_first_not_of(delimiter, 0);

	// Find the first non-delimiter
	string::size_type pos = str.find_first_of(delimiter, lastPos);

	// Tokenize by iterating through and finding delimiter positions
	while(string::npos != pos || string::npos != lastPos) {
		// Find a token and add it to the vector
		tokens.push_back(str.substr(lastPos, pos - lastPos));

		// Skip delimiters
		lastPos = str.find_first_not_of(delimiter, pos);

		// Find next non-delimiter
		pos = str.find_first_of(delimiter, lastPos);
	}
}