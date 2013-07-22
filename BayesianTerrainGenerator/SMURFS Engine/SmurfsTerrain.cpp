/**************************************************************************************************
* Title: SmurfsTerrain.cpp
* Authors: Gael Huber,
* Description: Generate a terrain given a set of parameters.
**************************************************************************************************/
#include "SmurfsTerrain.h"

/**************************************************************************************************
* Constructor
**************************************************************************************************/
Terrain::Terrain(void) {
	initialize();
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
Terrain::~Terrain(void) {
}

/**************************************************************************************************
* Initialize the variables and pointers
**************************************************************************************************/
void Terrain::initialize(void) {
	heightMap = new float*[0];
	heightMap = 0;

	// set the rgb values for the colors (perhapse read these in from file eventually?)
	snowColor = Vector3(255.0f/255.0f, 250.0f/255.0f, 250.0f/255.0f);
	rockColor = Vector3(151.0f/255.0f, 105.0f/255.0f, 79.0f/255.0f);
	iceColor = Vector3(175.0f/255.0f, 238.0f/255.0f, 238.0f/255.0f);
	grassColor = Vector3(35.0f/255.0f, 142.0f/255.0f, 35.0f/255.0f);
	sandColor = Vector3(255.0f/255.0f, 236.0f/255.0f, 139.0f/255.0f);
	waterColor = Vector3(0.0f, 0.0f, 156.0f/255.0f);
}

/**************************************************************************************************
* Generate a new terrain
**************************************************************************************************/
void Terrain::generateTerrain(void) {
	// Generate a height map
	generateHeightMap();

	// Generate the spline surface
	surface = new BSplineSurface();
	surface->build(heightMap, hmSize*hmSize + 1, hmSize*hmSize + 1, resolution);

	// Adjust the height
	adjustSize();

	// Populate a set of polygons 
	createPolygonGrid();
	
	// Generate the terrain types from the grid of terrain polygons
	generateTypes();

	//generateMesh();

	// Generate the mesh
	generateMeshFromPolygons();
}

/**************************************************************************************************
* Generate a height map of the terrain
**************************************************************************************************/
void Terrain::generateHeightMap(void) {
	// Read in the parameters used to load in the terrain
	readParameters();

	// Initialize array
	int arrayLength = (int)pow(2.0f, hmSize) + 1;
	heightMap = new float*[arrayLength];
	for(int i = 0; i < arrayLength; i++) {
		heightMap[i] = new float[arrayLength];
		for(int j = 0; j < arrayLength; j++) {
			heightMap[i][j] = 0.0f;
		}
	}

	// Define the initial square length, which should be the length of the entire grid
	int squareLength = arrayLength - 1;
	float range = 1.0f;		// Represents the amount by which a point can differ from its surrounding 
		// points, the total range is represented by plus/minus this number.

	// Iterate through and generate terrain properties until the square length is greater than zero
	while(squareLength > 1) {
		// Iterate through the array and perform the diamond operation
		for(int i = 0; i < arrayLength - 1; i += squareLength) {
			for(int j = 0; j < arrayLength - 1; j += squareLength) {
				// Find the four corners of the square we want to average
				float ul, ur, bl, br;
				ul = heightMap[i][j];
				ur = heightMap[i][j + squareLength];
				bl = heightMap[i + squareLength][j];
				br = heightMap[i + squareLength][j + squareLength];
				
				// Find the average elevation of those corners
				float avg = (ul + ur + bl + br) / 4.0f;

				// Generate a new elevation based on the current range and the elevation of the surrounding points
				float h = ((float)rand() / (float)RAND_MAX * (range * 2.0f)) - range;

				// Find the index of the midpoint of our square
				int midpoint[2] = {i + (squareLength / 2), j + (squareLength / 2)};

				// Assign the generated elevation plus the average value to the midpoint. This will yield a 
				// realistic offset of the terrain from its surrounding values.
				heightMap[midpoint[0]][midpoint[1]] = avg + h;
			}
		}

		// We want to define the whether midpoint should be offset in the i direction for our diamond
		bool midPointOffset = false;
		for(int i = 0; i < arrayLength; i += squareLength / 2) {
			// Toggle midPointOffset
			midPointOffset = !midPointOffset;
				
			for(int j = 0; j < arrayLength; j += squareLength) {
				// Define the midpoint we want to construct a diamond around
				int midpoint[2] = {i, j};

				// If toggle, adjust the j value by squareLength / 2
				if(midPointOffset) {
					midpoint[0] += squareLength / 2;
					midpoint[1] += squareLength / 2;
				}

				// Assuming the midpoint is within bounds
				if((midpoint[0] >= 0 && midpoint[0] < arrayLength) && 
					(midpoint[1] >= 0 && midpoint[1] < arrayLength)) {

					// Find the four corners of the diamond we want
					float u, d, l, r;
					int deviation = squareLength / 2;

					if(midpoint[1] - deviation < 0)
						u = heightMap[midpoint[0]][arrayLength - 1];
					else
						u = heightMap[midpoint[0]][midpoint[1] - deviation];

					if(midpoint[1] + deviation > arrayLength - 1)
						d = heightMap[midpoint[0]][0];
					else
						d = heightMap[midpoint[0]][midpoint[1] + deviation];
						
					if(midpoint[0] - deviation < 0)
						l = heightMap[arrayLength - 1][midpoint[1]];
					else
						l = heightMap[midpoint[0] - deviation][midpoint[1]];

					if(midpoint[0] + deviation > arrayLength - 1)
						r = heightMap[0][midpoint[1]];
					else
						r = heightMap[midpoint[0] + deviation][midpoint[1]];
						
					// Average the four points
					float avg = (u + d + l + r) / 4.0f;

					// Generate a new elevation based on the current range and the elevation of the surrounding points
					float h = ((float)rand() / (float)RAND_MAX * (range * 2.0f)) - range;

					// Assign the generated elevation plus the average value to the midpoint. This will yield a 
					// realistic offset of the terrain from its surrounding values.
					heightMap[midpoint[0]][midpoint[1]] = avg + h;
				}
			}
		}

		// Decrement the random number range
		range *= pow(2, -roughness);

		// Half the square length
		squareLength /= 2;
	}
}

/**************************************************************************************************
* Read in the attributes of the terrain
**************************************************************************************************/
void Terrain::readParameters(void) {
	// Load the parameters
	TerrainLoader tl = TerrainLoader();
	std::vector<float>* properties = tl.load("terrain1.ter");

	// Set the parameters
	hmSize = (int)(*properties)[0];
	resolution = (int)(*properties)[1];
	width = (*properties)[2];
	depth = (*properties)[3];
	maxHeight = (*properties)[4];
	minHeight = (*properties)[5];
	roughness = (*properties)[6];
}

/**************************************************************************************************
* Generates a sequence of vertices and sets colors based on the polygons
**************************************************************************************************/
void Terrain::generateMeshFromPolygons(void) {
	// The number of polygons in the array is the width x the height
	unsigned int polyNum = polyWidth * polyHeight;

	// Declare an array of all of the vertices to be displayed: 
	//	# of polygons * 3 vertices per polygon * 3 floats per vertex (x, y, z)
	float* polygonVertices = new float[polyNum * 9];
	// Declare an array of all of the colors:
	//	# of polygons * 3 vertices per polygon * 3 floats per vertex (RGB values)
	float* polygonColors = new float[polyNum * 9];
	// current index within the array
	unsigned int currentIndex = 0;

	for(int i = 0; i < polyHeight; i++) {
		for(int j = 0; j < polyWidth; j++) {
			// Get the color for the polygon
			Vector3 terrainColor = Vector3();
			SmurfPolygon* p = terrainPolygons[i][j];
			TERRAIN_TYPE t = terrainPolygons[i][j]->getType();

			switch(t) {
			case SNOW:
				terrainColor = snowColor;
				break;
			case ROCK:
				terrainColor = rockColor;
				break;
			case ICE:
				terrainColor = iceColor;
				break;
			case GRASS:
				terrainColor = grassColor;
				break;
			case SAND:
				terrainColor = sandColor;
				break;
			case WATER:
				terrainColor = waterColor;
				break;
			}


			// Point 1
			polygonVertices[currentIndex] = terrainPolygons[i][j]->getPoints()[0].x;
			polygonVertices[currentIndex + 1] = terrainPolygons[i][j]->getPoints()[0].y;
			polygonVertices[currentIndex + 2] = terrainPolygons[i][j]->getPoints()[0].z;

			// Set the appropriate color values
			polygonColors[currentIndex] = terrainColor.x; // R
			polygonColors[currentIndex + 1] = terrainColor.y; // G
			polygonColors[currentIndex + 2] = terrainColor.z; // B

			currentIndex += 3;

			// Point 2
			polygonVertices[currentIndex] = terrainPolygons[i][j]->getPoints()[1].x;
			polygonVertices[currentIndex + 1] = terrainPolygons[i][j]->getPoints()[1].y;
			polygonVertices[currentIndex + 2] = terrainPolygons[i][j]->getPoints()[1].z;
			
			polygonColors[currentIndex] = terrainColor.x; // R
			polygonColors[currentIndex + 1] = terrainColor.y; // G
			polygonColors[currentIndex + 2] = terrainColor.z; // B

			currentIndex += 3;

			// Point 3
			polygonVertices[currentIndex] = terrainPolygons[i][j]->getPoints()[2].x; 
			polygonVertices[currentIndex + 1] = terrainPolygons[i][j]->getPoints()[2].y;
			polygonVertices[currentIndex + 2] = terrainPolygons[i][j]->getPoints()[2].z;
			
			polygonColors[currentIndex] = terrainColor.x; // R
			polygonColors[currentIndex + 1] = terrainColor.y; // G
			polygonColors[currentIndex + 2] = terrainColor.z; // B
			
			currentIndex += 3;
		}
	}

	// Create the mesh based on the the vertices and the colors
	mesh = new Mesh(S_TRIANGLES);
	mesh->createMesh(polygonVertices, polygonColors, polyNum * 9);

	// Delete the arrays now since we don't need them anymore
	delete[] polygonVertices;
	delete[] polygonColors;

	mesh->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -28.0f));
}

/**************************************************************************************************
* Generate a sequence of vertices from the mesh, also generates colors
**************************************************************************************************/
void Terrain::generateMesh(void) {
	// Size of array will be determined by number of polygons able to be generated from the surface 
	// grid times 3
	// Number of polygons is area of array times two
	unsigned int polyNum = (surface->getWidth()-1) * (surface->getHeight()-1) * 2;
	
	// Declare the array, the number of polygons * 3 vertices per poly * 3 floats per vertex
	float* polygons = new float[polyNum * 9];
	float* colors = new float[polyNum * 9];
	float r, g, b;	// Color values for vertex
	r = g = b = 0.0f;
	unsigned int currentIndex = 0;	// The current index within the array

	for(unsigned int i = 0; i < surface->getWidth() - 1; i++) {
		for(unsigned int j = 0; j < surface->getHeight() - 1; j++) {
			// Top left
			polygons[currentIndex] = surface->getVertices()[i][j].x;
			polygons[currentIndex + 1] = surface->getVertices()[i][j].y;
			polygons[currentIndex + 2] = surface->getVertices()[i][j].z;

			// Calculate colors for appropriate height values
			calculateColorsFromHeight(polygons[currentIndex + 1], r, g, b);
			colors[currentIndex] = r; // R
			colors[currentIndex + 1] = g; // G
			colors[currentIndex + 2] = b; // B

			currentIndex += 3;

			// Top right
			polygons[currentIndex] = surface->getVertices()[i][j + 1].x;
			polygons[currentIndex + 1] = surface->getVertices()[i][j + 1].y;
			polygons[currentIndex + 2] = surface->getVertices()[i][j + 1].z;
			
			calculateColorsFromHeight(polygons[currentIndex + 1], r, g, b);
			colors[currentIndex] = r; // R
			colors[currentIndex + 1] = g; // G
			colors[currentIndex + 2] = b; // B

			currentIndex += 3;

			// Bottom left
			polygons[currentIndex] = surface->getVertices()[i + 1][j].x; 
			polygons[currentIndex + 1] = surface->getVertices()[i + 1][j].y;
			polygons[currentIndex + 2] = surface->getVertices()[i + 1][j].z;
			
			calculateColorsFromHeight(polygons[currentIndex + 1], r, g, b);
			colors[currentIndex] = r; // R
			colors[currentIndex + 1] = g; // G
			colors[currentIndex + 2] = b; // B
			
			currentIndex += 3;
			
			// Bottom left
			polygons[currentIndex] = surface->getVertices()[i + 1][j].x; 
			polygons[currentIndex + 1] = surface->getVertices()[i + 1][j].y;
			polygons[currentIndex + 2] = surface->getVertices()[i + 1][j].z;
			
			calculateColorsFromHeight(polygons[currentIndex + 1], r, g, b);
			colors[currentIndex] = r; // R
			colors[currentIndex + 1] = g; // G
			colors[currentIndex + 2] = b; // B
			
			currentIndex += 3;
			
			// Top right
			polygons[currentIndex] = surface->getVertices()[i][j + 1].x; 
			polygons[currentIndex + 1] = surface->getVertices()[i][j + 1].y;
			polygons[currentIndex + 2] = surface->getVertices()[i][j + 1].z;
			
			calculateColorsFromHeight(polygons[currentIndex + 1], r, g, b);
			colors[currentIndex] = r; // R
			colors[currentIndex + 1] = g; // G
			colors[currentIndex + 2] = b; // B
			
			currentIndex += 3;
			
			// Bottom right
			polygons[currentIndex] = surface->getVertices()[i + 1][j + 1].x; 
			polygons[currentIndex + 1] = surface->getVertices()[i + 1][j + 1].y;
			polygons[currentIndex + 2] = surface->getVertices()[i + 1][j + 1].z;
			
			calculateColorsFromHeight(polygons[currentIndex + 1], r, g, b);
			colors[currentIndex] = r; // R
			colors[currentIndex + 1] = g; // G
			colors[currentIndex + 2] = b; // B
			
			currentIndex += 3;
		}
	}

	// Create the mesh based on the the vertices and the colors
	mesh = new Mesh(S_TRIANGLES);
	mesh->createMesh(polygons, colors, polyNum * 9);

	// Delete the arrays now since we don't need them anymore
	delete[] polygons;
	delete[] colors;

	mesh->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -28.0f));
}

/**************************************************************************************************
* Take a height value and based on the range of possible heights determine its RGB values
**************************************************************************************************/
void Terrain::calculateColorsFromHeight(float height, float& red, float& green, float& blue) {
	// This height is what percent of the maximum height
	float percent = (height - minHeight) / (maxHeight - minHeight);

	// Multiply this percentile by a value of 765 (255 * 4) to determine its cummulative RGB value
	int rgb = (int)(percent * 1020.0f);
	
	// Default to blue for lowest color range
	int r, g, b;
	r = g = 0;
	b = 255;

	// While the RGB value is greater than 255 (maximum value of any given color), decrement it by 255
	// and increment color, effectively telling us that we want to consider the next color in the line
	while(rgb > 0) {
		// Blue -> Teal
		if(b == 255 && g < 255)
			g++;
		// Teal -> Green
		else if(g == 255 && b > 0)
			b--;
		// Green -> Yellow
		else if(g == 255 && r < 255)
			r++;
		// Yellow -> Red
		else if(r == 255 && g > 0)
			g--;

		// Decrement RGB
		rgb--;
	}

	// Assign the colors
	red = (float) r / 255.0f;
	green = (float) g / 255.0f;
	blue = (float) b / 255.0f;
}

/**************************************************************************************************
* Adjust the size of the terrain to fix the appropriate max and min
**************************************************************************************************/
void Terrain::adjustSize(void) {
	// Calculate the range of the terrain's height
	float range = maxHeight - minHeight;

	// Iterate through the terrain
	for(unsigned int i = 0; i < surface->getHeight(); i++) {
		for(unsigned int j = 0; j < surface->getWidth(); j++) {
			Vector3 tesat = surface->getVertices()[i][j];
			surface->getVertices()[i][j].x = ((surface->getVertices()[i][j].x * 2.0f) - 1.0f) * width;
			surface->getVertices()[i][j].y = (surface->getVertices()[i][j].y + 1.0f) / 2.0f * range + minHeight;
			surface->getVertices()[i][j].z = ((surface->getVertices()[i][j].z * 2.0f) - 1.0f) * depth;
		}
	}

}

/**************************************************************************************************
* Go through the surface and create a set of polygons
**************************************************************************************************/
void Terrain::createPolygonGrid(void) {
	// Size of array will be determined by number of polygons able to be generated from the surface 
	// grid times 3
	// Number of polygons is area of array times two
	unsigned int polyNum = (surface->getWidth()-1) * (surface->getHeight()-1) * 2;
	polyHeight = surface->getHeight() - 1;
	polyWidth = (surface->getWidth() - 1) * 2;

	// Declare and initialize grid of polygons
	terrainPolygons = new SmurfPolygon**[polyHeight];
	for(int i = 0; i < polyHeight; i++) {
		terrainPolygons[i] = new SmurfPolygon*[polyWidth];
	}

	// Current position in the array
	unsigned int cWidth = 0;
	unsigned int cHeight = 0;

	// Iterate through the grid and assign vertices
	for(unsigned int i = 0; i < surface->getHeight() - 1; i++) {
		for(unsigned int j = 0; j < surface->getWidth() - 1; j++) {
			// Three points
			Vector3 v[3];

			// Populate the points
			// Top left
			v[0] = Vector3(surface->getVertices()[i][j].x, 
				surface->getVertices()[i][j].y, 
				surface->getVertices()[i][j].z);

			// Top right
			v[1] = Vector3(surface->getVertices()[i][j + 1].x,
				surface->getVertices()[i][j + 1].y,
				surface->getVertices()[i][j + 1].z);

			// Bottom left
			v[2] = Vector3(surface->getVertices()[i + 1][j].x,
				surface->getVertices()[i + 1][j].y,
				surface->getVertices()[i + 1][j].z);
			
			// Create a polygon
			terrainPolygons[cHeight][cWidth++] = new SmurfPolygon(v);

			// Bottom left
			v[0] = Vector3(surface->getVertices()[i + 1][j].x,
				surface->getVertices()[i + 1][j].y,
				surface->getVertices()[i + 1][j].z);

			// Top right
			v[1] = Vector3(surface->getVertices()[i][j + 1].x,
				surface->getVertices()[i][j + 1].y,
				surface->getVertices()[i][j + 1].z);

			// Bottom right
			v[2] = Vector3(surface->getVertices()[i + 1][j + 1].x,
				surface->getVertices()[i + 1][j + 1].y,
				surface->getVertices()[i + 1][j + 1].z);
			
			// Create a second polygon
			terrainPolygons[cHeight][cWidth++] = new SmurfPolygon(v);
		}

		// Reset width and increment height
		cWidth = 0;
		cHeight++;
	}
}

/**************************************************************************************************
* Generate terrain types based on Bayesian reasoning
**************************************************************************************************/
void Terrain::generateTypes(void) {
	// create the terrain analyzer
	TerrainAnalyzer analyzer = TerrainAnalyzer();
	
	// analyze the terrain
	analyzer.analyze(terrainPolygons, polyWidth, polyHeight, maxHeight, minHeight);
}

/**************************************************************************************************
* Getters and setters
**************************************************************************************************/
Mesh* Terrain::getMesh(void) { return mesh; }