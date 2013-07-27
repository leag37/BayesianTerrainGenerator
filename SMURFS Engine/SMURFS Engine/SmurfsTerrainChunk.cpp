
#include "SmurfsTerrainChunk.h"

TerrainChunk::TerrainChunk()
{

}

TerrainChunk::TerrainChunk(HeightMap* hm, FLOAT startX, FLOAT startZ, FLOAT xWidth, FLOAT zWidth)
	:	_hm(hm), _xStart(startX), _zStart(startZ), _xWidth(xWidth), _zWidth(zWidth)
{
}


Mesh* TerrainChunk::mesh()
{
	return _mesh;
}

// Generate a mesh from a height map
void TerrainChunk::genMeshFromHeightMap()
{
	// TEMP
	snow = Vector3(255.0f/255.0f, 250.0f/255.0f, 250.0f/255.0f);
	rock = Vector3(151.0f/255.0f, 105.0f/255.0f, 79.0f/255.0f);
	grass = Vector3(35.0f/255.0f, 142.0f/255.0f, 35.0f/255.0f);
	sand = Vector3(255.0f/255.0f, 236.0f/255.0f, 139.0f/255.0f);
	water = Vector3(0.0f, 0.0f, 156.0f/255.0f);

	// END TEMP

	// Calculate the number of polygons
	UINT numPolygons = _hm->resolution() * _hm->resolution() << 1;

	// Get array of vertices
	FLOAT* verts = new FLOAT[numPolygons * 9];

	// Get array of polygon colors
	FLOAT* colors = new FLOAT[numPolygons * 9];
	FLOAT colorScale = (_hm->maxHeight() - _hm->minHeight());
	FLOAT colorOffset = -_hm->minHeight();

	// Current polygon index
	UINT currPolyIndex = 0;

	// Get the heightmap
	FLOAT** hm = _hm->heightMap();

	// Get x aspect ration
	FLOAT xRatio = _xWidth / _hm->resolution();
	FLOAT zRatio = _zWidth / _hm->resolution();

	for(UINT i = 0; i < _hm->resolution(); ++i)
	{
		// Convert i to x value
		FLOAT x1 = static_cast<FLOAT>(i) * xRatio;
		FLOAT x2 = static_cast<FLOAT>(i + 1) * xRatio;
		for(UINT j = 0; j < _hm->resolution(); ++j)
		{
			// Get the z value for the first polygon-square
			FLOAT z1 = static_cast<FLOAT>(j) * zRatio;
			FLOAT z2 = static_cast<FLOAT>(j + 1) * zRatio;

			// Grab y for all 4 points
			FLOAT y1 = hm[i    ][j    ];
			FLOAT y2 = hm[i + 1][j    ];
			FLOAT y3 = hm[i + 1][j + 1];
			FLOAT y4 = hm[i    ][j + 1];

			// Assign colors
			FLOAT wx1 = x1 / _xWidth;;
			FLOAT wx2 = x2 / _xWidth;
			FLOAT wz1 = z1 / _zWidth;
			FLOAT wz2 = z2 / _zWidth;
			Vector3 color1 = assignColor(wx1, (y1 + colorOffset) / colorScale, wz1);
			Vector3 color2 = assignColor(wx2, (y2 + colorOffset) / colorScale, wz1);
			Vector3 color3 = assignColor(wx1, (y3 + colorOffset) / colorScale, wz2);
			Vector3 color4 = assignColor(wx1, (y4 + colorOffset) / colorScale, wz2);

			// Generate color and coordinates for point 1
			verts[currPolyIndex    ] = x1;
			verts[currPolyIndex + 1] = y1;
			verts[currPolyIndex + 2] = z1;
			
			colors[currPolyIndex    ] = color1.x;
			colors[currPolyIndex + 1] = color1.y;
			colors[currPolyIndex + 2] = color1.z;

			currPolyIndex += 3;

			// Generate color and coordinates for point 2
			verts[currPolyIndex    ] = x2;
			verts[currPolyIndex + 1] = y2;
			verts[currPolyIndex + 2] = z1;
			
			colors[currPolyIndex    ] = color2.x;
			colors[currPolyIndex + 1] = color2.y;
			colors[currPolyIndex + 2] = color2.z;

			currPolyIndex += 3;

			// Generate color and coordinates for point 3
			verts[currPolyIndex    ] = x2;
			verts[currPolyIndex + 1] = y3;
			verts[currPolyIndex + 2] = z2;
			
			colors[currPolyIndex    ] = color3.x;
			colors[currPolyIndex + 1] = color3.y;
			colors[currPolyIndex + 2] = color3.z;
			
			currPolyIndex += 3;

			// Generate color and coordinates for point 4
			verts[currPolyIndex    ] = x1;
			verts[currPolyIndex + 1] = y1;
			verts[currPolyIndex + 2] = z1;
			
			colors[currPolyIndex    ] = color1.x;
			colors[currPolyIndex + 1] = color1.y;
			colors[currPolyIndex + 2] = color1.z;
			
			currPolyIndex += 3;

			// Generate color and coordinates for point 5
			verts[currPolyIndex    ] = x2;
			verts[currPolyIndex + 1] = y3;
			verts[currPolyIndex + 2] = z2;
			
			colors[currPolyIndex    ] = color3.x;
			colors[currPolyIndex + 1] = color3.y;
			colors[currPolyIndex + 2] = color3.z;

			currPolyIndex += 3;

			// Generate color and coordinates for point 6
			verts[currPolyIndex    ] = x1;
			verts[currPolyIndex + 1] = y4;
			verts[currPolyIndex + 2] = z2;
			
			colors[currPolyIndex    ] = color4.x;
			colors[currPolyIndex + 1] = color4.y;
			colors[currPolyIndex + 2] = color4.z;

			currPolyIndex += 3;
		}
	}

	
	// Create the mesh based on the the vertices and the colors
	_mesh = new Mesh(S_TRIANGLES);
	_mesh->createMesh(verts, colors, numPolygons * 9);
	delete[] verts;
	delete[] colors;

	_mesh->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(_hm->startX(), 0.0f, _hm->startZ()));
}

Vector3 TerrainChunk::assignColor(FLOAT x, FLOAT y, FLOAT z)
{
	if(y < 0.25)		return water;
	else if(y < 0.40)	return sand;
	else if(y < 0.70)	return grass;
	else if(y < 0.95)	return rock;
	else				return snow;
}



