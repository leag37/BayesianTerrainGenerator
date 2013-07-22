/***************************************************************************
* Title: TerrainLoader.h
* Author(s): Erin Mooney
* Description: Reads in information about creating the terrain from a file
***************************************************************************/
#include "TerrainLoader.h"

/***************************************************************************
* Constructor
***************************************************************************/
TerrainLoader::TerrainLoader(void) {
	FILEPATH = "..\\media\\terrain\\";
}

/***************************************************************************
* Destructor
***************************************************************************/
TerrainLoader::~TerrainLoader(void) { }

/***************************************************************************
* Reads the file and sets the correct variables
* 
* Filename		the name of the file we want to load
***************************************************************************/
std::vector<float>* TerrainLoader::load(std::string filename) {
	// variable for reading in
	std::string line;

	// vector to be returned
	std::vector<float>* data = new std::vector<float>();
	
	// create the stream reader
	std::ifstream file (FILEPATH + filename);

	// parse the information
	if(file.is_open()) {
		while(file.good()) {
			std::getline(file, line);
			// if the line is the diamond square resolution
			if(line == "Diamond Resolution") {
				std::getline(file, line);
				data->push_back((float) std::atof(line.c_str()));
			} else if(line == "B Spline Resolution") {
				std::getline(file, line);
				data->push_back((float) std::atof(line.c_str()));
			} else if(line == "X Dimension") {
				std::getline(file, line);
				data->push_back((float) std::atof(line.c_str()));
			} else if(line == "Z Dimension") {
				std::getline(file, line);
				data->push_back((float) std::atof(line.c_str()));
			} else if(line == "Max Height") {
				std::getline(file, line);
				data->push_back((float) std::atof(line.c_str()));
			} else if(line == "Min Height") {
				std::getline(file, line);
				data->push_back((float) std::atof(line.c_str()));
			} else if(line == "Roughness") {
				std::getline(file, line);
				data->push_back((float) std::atof(line.c_str()));
			}
		}
	}
	// return the array
	return data;
}