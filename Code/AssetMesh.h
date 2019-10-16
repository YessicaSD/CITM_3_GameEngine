#ifndef __ASSET_MESH_H__
#define __ASSET_MESH_H__

#include "MathGeoLib/include/Math/float3.h"
#include "Assimp/include/vector3.h"


typedef unsigned int uint;
struct aiMesh;

class AssetMesh
{
public:
	//Data ========================
	uint id_indice = 0u; // index in VRAM
	uint num_indices = 0u;
	uint16_t * indices = nullptr;
	//TODO: Change to uint *
	//Assimp uses uint * and par_shapes uses uint16_t *

	//Vertec ================================
	uint id_vertex = 0u; // unique vertex in VRAM
	uint num_vertices = 0u;
	float* vertices = nullptr;

	//Normals ===================
	float3* normals = nullptr;
	uint numFaces = 0u;
	//TODO: We don't need float 3 pointer, we can do it with float, as the vertices
	float3* faces_normals = nullptr;
	float3* face_middle_point = nullptr;


	// UV ==================
	float* UVCoord = nullptr;
	uint id_uv = 0;
	uint uv_num_components = 0;

	uint id_texture;

	bool LoadInfo(aiMesh* info);
	bool GenereteBuffer(aiMesh* info);

};


#endif