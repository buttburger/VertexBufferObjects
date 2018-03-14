#ifndef _OBJECT_H
#define _OBJECT_H
#define MAX_VERTICES 800000 //Max number of vertices(for each object)
#define MAX_POLYGONS 800000 //Max number of polygons(for each object)
#define MAX_OBJECTS 100
#define MAX_VALUE 100
char name[20];
int num_textures[1000];
int vertices_quantity;
int polygons_quantity;
struct vertex
{
	float x, y, z;
}vertices[MAX_VERTICES], normal[MAX_VERTICES], controlpoint[MAX_VERTICES];
//Array of vertices & normals
//The polygon(triangle), 3 numbers that aim 3 vertices
struct polygon
{
	unsigned short a, b, c;
}polygons[MAX_POLYGONS];
//Array of polygons (numbers that point to the vertices' list)
// The mapcoord type, 2 texture coordinates for each vertex
struct map
{
	float u[123][3], v[123][3];
}mapcoords;
// Array of UV coordinates for texture mapping
struct matrix
{
	float x[MAX_OBJECTS][MAX_VALUE][3], y[MAX_OBJECTS][MAX_VALUE][3], z[MAX_OBJECTS][MAX_VALUE][3];
	float xx[MAX_VERTICES][3], yy[MAX_VERTICES][3], zz[MAX_VERTICES][3];
}LclT, LclR, LclS, matT, matR, matS, bposeT, bposeR, bposeS, maT, maR, maS;

/*
struct Joint
{
	//string Name;
	int ParentIndex;
	FbxAMatrix GlobalBindposeInverse;
	FbxAMatrix LocalTransform;
	FbxAMatrix GlobalTransform;
	FbxNode* Node;
};
*/

#endif