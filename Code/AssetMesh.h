#ifndef __ASSET_MESH_H__
#define __ASSET_MESH_H__

#include "MathGeoLib/include/Math/float3.h"
#include "Assimp/include/vector3.h"

typedef unsigned int uint;

class AssetMesh
{
public:
	uint id_indice = 0u; // index in VRAM
	uint num_indices = 0u;
	uint* indices = nullptr;

	uint id_vertex = 0u; // unique vertex in VRAM
	uint num_vertices = 0u;
	float* vertices = nullptr;

	aiVector3D* normals = nullptr;
	uint numFaces = 0u;
	//TODO: We don't need float 3 pointer, we can do it with float, as the vertices
	float3* faces_normals = nullptr;
	float3* face_middle_point = nullptr;

};


#endif