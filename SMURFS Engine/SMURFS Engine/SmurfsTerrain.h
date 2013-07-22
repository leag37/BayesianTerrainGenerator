/**************************************************************************************************
* Title: SmurfsTerrain.h
* Authors: Gael Huber,
* Description: Generate a terrain given a set of parameters.
**************************************************************************************************/
#ifndef __SMURFSTERRAIN_H__
#define __SMURFSTERRAIN_H__

#include <random>
#include "SmurfsBSplineSurface.h"
#include "SmurfsMesh.h"
#include "TerrainLoader.h"
#include "SmurfsTerrainAnalyzer.h"
#include "SmurfsPolygon.h"
#include "SmurfsVector3.h"

class Terrain {
public:
	Terrain(void);		// Constructor
	~Terrain(void);		// Destructor

	void generateTerrain(void);		// Generate a new terrain

	// Getters
	Mesh* getMesh(void);

private:
	void initialize(void);	// Initialize the variables and pointers
	void generateHeightMap(void);	// Generate a height map of the terrain
	void readParameters(void);		// Read parameters used to construct the terrain from a file
	void generateMesh(void);	// Generate a sequence of vertices from the mesh, also generates colors
	void generateMeshFromPolygons(void);	// Generates a sequence of vertices and sets colors based on the polygons
	void calculateColorsFromHeight(float height, float& red, float& green, float& blue);	// Take a
		// height value and based on the range of possible heights determine its RGB values
	void adjustSize(void);	// Adjust the size of the terrain to fix the appropriate max and min
	void createPolygonGrid(void);	// Go through the surface and create a set of polygons
	void generateTypes(void);	// Generate terrain types based on Bayesian reasoning

	float** heightMap;	// Stores data points for the heightmap
	int hmSize;		// "Size" of the heightmap, i.e. the width/height
	float roughness;	// Roughness value used when generating terrain
	float maxHeight;	// Maximum height value of the actual terrain
	float minHeight;	// Minimum height value of the actual terrain
	float width;	// Physical width of the world in world coordinates
	float depth;	// Physical depth of the world in world coordinates
	int resolution;		// Resolution value for how many polygons to create from the heightmap
	BSplineSurface* surface;	// B-spline used to build the smooth terrain
	Mesh* mesh;		// Mesh associated with this object

	SmurfPolygon*** terrainPolygons;		// Grid of polygons
	int polyWidth;		// Width of polygon array
	int polyHeight;		// Height of polygon array

	Vector3 snowColor;		// off-white
	Vector3 rockColor;		// brown
	Vector3 iceColor;		// light purple
	Vector3 grassColor;		// green
	Vector3 sandColor;		// tan
	Vector3 waterColor;		// blue
};


#endif