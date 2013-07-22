/**************************************************************************************************
* Title: SmurfsTerrainAnalyzer.cpp
* Authors: Gael Huber, Erin Mooney
* Description: Analyzes a terrain given a set of polygons, will then run analysis on invidivual 
*	pieces using bayesian networks and naive bayes.
**************************************************************************************************/
#include "SmurfsTerrainAnalyzer.h"

/**************************************************************************************************
* Constructor
**************************************************************************************************/
TerrainAnalyzer::TerrainAnalyzer(void) {
	// Load in the matrices
	loadMatrices();
}
	
/**************************************************************************************************
* Destructor
**************************************************************************************************/
TerrainAnalyzer::~TerrainAnalyzer(void) {
}
	
/**************************************************************************************************
* Analyze the terrain and assign the proper type
**************************************************************************************************/
void TerrainAnalyzer::analyze(SmurfPolygon*** terrain, int w, int h, float maxHeight, float minHeight) {
	// Iterate through the terrain and analyze each individual terrain piece of terrain
	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j+=2) {
			// Clear the probabilities
			clearProbabilities();
			
			// Choose the a piece of terrain and analyze it
			//**********************************************************************************************
			// Analyze slope
			analyzeSlope(terrain[i][j]);

			// Analyze height
			analyzeHeight(terrain[i][j], maxHeight, minHeight);

			/* Analyze adjacent terrains
			* Even
			*	i-1: j-1, j, j+1, j+2, j+3
			*	i: j-2, j-1
			* Odd 
			*	i-1: j, j+1, j+2
			*	i: j-2, j-1
			*/
			int adjacentAnalyzed = 0;
			if(i % 2 == 0) {	// Even
				// case: i - 1
				if(i - 1 >= 0) {
					if(j - 1 >= 0) {
						analyzeAdjacent(terrain[i][j], terrain[i-1][j-1]);
						adjacentAnalyzed++;
					}

					analyzeAdjacent(terrain[i][j], terrain[i-1][j]);
					adjacentAnalyzed++;

					if(j + 1 < w) {
						analyzeAdjacent(terrain[i][j], terrain[i-1][j+1]);
						adjacentAnalyzed++;
					}
					if(j + 2 < w) {
						analyzeAdjacent(terrain[i][j], terrain[i-1][j+2]);
						adjacentAnalyzed++;
					}
					if(j + 3 < w) {
						analyzeAdjacent(terrain[i][j], terrain[i-1][j+3]);
						adjacentAnalyzed++;
					}
				}

				// case: i
				if(j - 2 >= 0) {
					analyzeAdjacent(terrain[i][j], terrain[i][j-2]);
					adjacentAnalyzed++;
				}
				if(j - 1 >= 0) {
					analyzeAdjacent(terrain[i][j], terrain[i][j-1]);
					adjacentAnalyzed++;
				}
			} else {	// Odd
				// case: i - 1
				if(i - 1 >= 0) {
					analyzeAdjacent(terrain[i][j], terrain[i-1][j]);
					adjacentAnalyzed++;

					if(j + 1 < w) {
						analyzeAdjacent(terrain[i][j], terrain[i-1][j+1]);
						adjacentAnalyzed++;
					}
					if(j + 2 < w) {
						analyzeAdjacent(terrain[i][j], terrain[i-1][j+2]);
						adjacentAnalyzed++;
					}
				}

				// case: i
				if(j - 2 >= 0) {
					analyzeAdjacent(terrain[i][j], terrain[i][j-2]);
					adjacentAnalyzed++;
				}
				if(j - 1 >= 0) {
					analyzeAdjacent(terrain[i][j], terrain[i][j-1]);
					adjacentAnalyzed++;
				}					
			}

			if(adjacentAnalyzed == 0)
				adjacentAnalyzed = 1;
			
			// Find the average probability of the adjacent values
			for(int k = 0; k < 6; k++) {
				// Edge case testing. This is to prevent a band of the most probable substance at the seeding 
				// corner, essentially this causes the entire first row of values to act as a seed rather than the first value.
				if(i == 0)
					terrainProbabilities[k][3] = 0.1f;
				else
					terrainProbabilities[k][3] = terrainProbabilities[k][3] / (float)adjacentAnalyzed;
			}

			// Use naive bayes for final analysis of probabilities
			naiveBayes();
			
			// assign terrain type
			assignType(terrain[i][j]);
			assignType(terrain[i][j+1]);
		}
	}
}
	
/**************************************************************************************************
* Load the correct matrices into memory
**************************************************************************************************/
void TerrainAnalyzer::loadMatrices(void) {	
	height = new Matrix();
	*height = MatrixReader::getSingletonPtr()->loadMatrix("height.mat");

	slope = new Matrix();
	*slope = MatrixReader::getSingletonPtr()->loadMatrix("slope.mat");

	adjacent = new Matrix();
	*adjacent = MatrixReader::getSingletonPtr()->loadMatrix("adjacent.mat");
}
	
/**************************************************************************************************
* Analyze the slope of the terrain
**************************************************************************************************/
void TerrainAnalyzer::analyzeSlope(SmurfPolygon* terrain) {
	// Calculate the inverse matrix, this gives us a value between 0 and 1
	float inverseSlope = terrain->getSlope() / SmurfPolygon::maxSlope;

	// Create an array of length 6 and calculate the proportionality of each terrain
	float proportionals[6];

	// Find out what kind of slope it is
	int s = 0;
	if(inverseSlope <= 0.20f)
		s = 0;	// Flat
	else if(inverseSlope <= 0.35f)
		s = 1; // Shallow
	else if(inverseSlope <= 0.60f)
		s = 2;	// Hilly
	else
		s = 3;	// Mountainous

	// Iterate through each terrain type and calculate the proportionality and cumulate the sum
	float sum = 0.0f;
	for(int i = 0; i < 6; i++) {
		proportionals[i] = slope->matrix[s][i];
		sum += proportionals[i];
	}

	// Find the probabilities
	for(int i = 0; i < 6; i++) {
		terrainProbabilities[i][1] = proportionals[i] / sum;
	}
}
	
/************************************************************************************************** 0 
* Analyze the height of the terrain
**************************************************************************************************/
void TerrainAnalyzer::analyzeHeight(SmurfPolygon* terrain, float maxHeight, float minHeight) {
	// find the height range
	float heightRange = SmurfPolygon::maxHeight - SmurfPolygon::minHeight;

	// calculate the center
	float center = SmurfPolygon::maxHeight - (heightRange/2.0f);

	// calculate the max height for the 4 different ranges: valley, lowlands, foothills, mountains
	float mountainsMax = center + (heightRange / 2.0f);
	float foothillsMax = center + (heightRange / 8.0f);
	float lowlandsMax = center;
	float valleyMax = center - (heightRange / 8.0f);

	// find the height of the current piece of terrain: 0 = valley, 1 = lowlands, 2 = foothills, 3 = mountains
	float currTerrainCenter = terrain->getCenter().y;
	int currTerrainHeight = -1;
	// valley is between minHeight and valleyMax
	if(currTerrainCenter >= SmurfPolygon::minHeight && currTerrainCenter <= valleyMax)
		currTerrainHeight = 0;
	// lowlands are between valleyMax and lowlandsMax
	else if(currTerrainCenter > valleyMax && currTerrainCenter <= lowlandsMax)
		currTerrainHeight = 1;
	// foothills are between lowlandsMax and foothillsMax
	else if(currTerrainCenter > lowlandsMax && currTerrainCenter <= foothillsMax)
		currTerrainHeight = 2;
	// mountains are between foothillsMax and mountainsMax
	else if(currTerrainCenter > foothillsMax && currTerrainCenter <= mountainsMax)
		currTerrainHeight = 3;
	
	/*	Height Matrix:
					snow	rock	ice		grass	sand	water
		valley		 -		 -		 -		 -		 -		 -
		lowlands	 -		 -		 -		 -		 -		 -
		foothills	 -		 -		 -		 -		 -		 -
		mountains	 -		 -		 -		 -		 -		 -

		Terrain Probabilities Matrix:
					total	slope	height	adjacent
		snow		 -		 -		 -		 -
		rock		 -		 -		 -		 -
		ice			 -		 -		 -		 -
		grass		 -		 -		 -		 -
		sand		 -		 -		 -		 -
		water		 -		 -		 -		 -
	*/

	// go through each of the different pieces of terrain and figure out the probability of
	//	height based on the height matrix, set the appropriate values in terrainProabilities
	float sum = 0.0f;
	for(int i = 0; i < 6; i++) {
		terrainProbabilities[i][2] = height->matrix[currTerrainHeight][i];
		sum += terrainProbabilities[i][2];
	}

	// Find the probabilities
	for(int i = 0; i < 6; i++) {
		terrainProbabilities[i][2] /= sum;
	}
}
	
/**************************************************************************************************
* Analyze the adjacent terrain
**************************************************************************************************/
void TerrainAnalyzer::analyzeAdjacent(SmurfPolygon* terrain, SmurfPolygon* known) {
	// get the terrain type of the known polygon
	TERRAIN_TYPE t = known->getType();
	int knownType = -1;
	switch(t) {
	case SNOW:
		knownType = 0;
		break;
	case ROCK:
		knownType = 1;
		break;
	case ICE:
		knownType = 2;
		break;
	case GRASS:
		knownType = 3;
		break;
	case SAND:
		knownType = 4;
		break;
	case WATER:
		knownType = 5;
		break;
	}

	// loop through and determine the probabilities based on the adjacent matrix, set the appropriate values
	float sum = 0.0f;
	for(int i = 0; i < 6; i++) {
		terrainProbabilities[i][3] += adjacent->matrix[knownType][i];
		sum += terrainProbabilities[i][3];
	}

	// Find the probabilities
	for(int i = 0; i < 6; i++) {
		terrainProbabilities[i][3] /= sum;
	}
}

/**************************************************************************************************
* Clear out the probabilities currently stored
**************************************************************************************************/
void TerrainAnalyzer::clearProbabilities(void) {
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 4; j++)
			terrainProbabilities[i][j] = 0.0f;
}

/**************************************************************************************************
* Use Naive Bayes for the final analysis of probability
**************************************************************************************************/
void TerrainAnalyzer::naiveBayes(void) {	
	// loop through the different types of terrain and sum up their probabilities and divide by 3
	float totalProb;
	float slopeProb;
	float heightProb;
	float adjacentProb;
	for(int i = 0; i < 6; i++) {
		slopeProb = terrainProbabilities[i][1];
		heightProb = terrainProbabilities[i][2];
		adjacentProb = terrainProbabilities[i][3];
		totalProb = slopeProb + heightProb + adjacentProb;
		terrainProbabilities[i][0] = totalProb / 3;
	}
}

/**************************************************************************************************
* Use the log likelihood method, return the most likely terrain
**************************************************************************************************/
int TerrainAnalyzer::logLikelihood(void) {
	// loop through and find terrain type with the highest total
	int terrainType = -1;
	float terrainTotal = 0.0f;
	for(int i = 0; i < 6; i++) {
		float currentTotal = log(terrainProbabilities[i][0]);
		if(terrainType == -1 || currentTotal >= terrainTotal) {
			terrainType = i;
			terrainTotal = currentTotal;
		}
	}
	// return the most likely terrain
	return terrainType;
}

/**************************************************************************************************
* Use the babbler method, return the most likely terrain
**************************************************************************************************/
int TerrainAnalyzer::babbler(void) { 
	// find the total probabilities
	float totalProbability = 0.0f;
	for(int i = 0; i < 6; i++) {
		totalProbability += terrainProbabilities[i][0];
	}
	// random number between 0 and 1
	float random = ((float)rand()) / ((float)RAND_MAX);

	// loop through and find the terrain value at the random value
	float currentValue = 0.0f;
	for(int i = 0; i < 6; i++) {
		currentValue += terrainProbabilities[i][0] / totalProbability;

		if(currentValue >= random) {
			return i;	// return the terrain type
		}
	}

	// didn't find a terrain type (aka there was an error)
	return -1;
}

/**************************************************************************************************
* Assign a terrain type to a polygon based on likelihoods
**************************************************************************************************/
void TerrainAnalyzer::assignType(SmurfPolygon* terrain) {
	// use the loglikelihood method
	int finalType = logLikelihood();

	// set the terrain type
	switch(finalType) {
	case 0:
		terrain->setType(SNOW);
		break;
	case 1:
		terrain->setType(ROCK);
		break;
	case 2:
		terrain->setType(ICE);
		break;
	case 3:
		terrain->setType(GRASS);
		break;
	case 4:
		terrain->setType(SAND);
		break;
	case 5:
		terrain->setType(WATER);
		break;
	}
}