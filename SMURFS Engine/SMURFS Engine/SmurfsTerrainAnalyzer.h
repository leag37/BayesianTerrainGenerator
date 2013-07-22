/**************************************************************************************************
* Title: SmurfsTerrainAnalyzer.h
* Authors: Gael Huber, Erin Mooney
* Description: Analyzes a terrain given a set of polygons, will then run analysis on invidivual 
*	pieces using bayesian networks and naive bayes.
**************************************************************************************************/
#ifndef __SMURFSTERRAINANALYZER_H__
#define __SMURFSTERRAINANALYZER_H__

#include "SmurfsPolygon.h"
#include "SmurfsMatrixReader.h"
#include <random>

class TerrainAnalyzer {
public:
	TerrainAnalyzer(void);		// Constructor
	~TerrainAnalyzer(void);		// Destructor

	void analyze(SmurfPolygon*** terrain, int w, int h, float maxHeight, float minHeight);		// Analyze the terrain and assign the proper type

private:
	void loadMatrices(void);	// Load the correct matrices into memory
	void analyzeSlope(SmurfPolygon* terrain);	// Analyze the slope of the terrain
	void analyzeHeight(SmurfPolygon* terrain, float maxHeight, float minHeight);	// Analyze the height of the terrain
	void analyzeAdjacent(SmurfPolygon* terrain, SmurfPolygon* known);		// Analyze the adjacent terrain
	void clearProbabilities(void);		// Clear the probabilities of the array
	void naiveBayes(void);		// Use Naive Bayes for the final analysis of probability
	int logLikelihood(void);	// use the log likelihood method, return the most likely terrain
	int babbler(void);			// use the babbler method, return the most likely terrain
	void assignType(SmurfPolygon* terrain);		// Assign a terrain type to a polygon based on likelihoods

	Matrix* slope;		// Matrix relating terrain type to slope
	Matrix* height;		// Matrix relating terrain type to height
	Matrix* adjacent;	// Matrix relating terrain type to adjacent terrain types
	float terrainProbabilities[6][4];	// Probabilities of each terrain type, formatted in the following way:
		// total, slope, height, adjacent
};

#endif