#include "AssetMesh.h"
#include "Globals.h"

bool AssetMesh::LoadMesh(uint num_vertices, float * mVetices)
{
	this->num_vertices = num_vertices;
	this->vertices = new float[num_vertices * 3];
	memcpy(this->vertices, mVetices, sizeof(float) *num_vertices * 3);
	LOG("New mesh with %d vertices", num_vertices);
	return true;
}