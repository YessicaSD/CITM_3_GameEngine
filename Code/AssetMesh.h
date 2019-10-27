#ifndef __ASSET_MESH_H__
#define __ASSET_MESH_H__

#include "MathGeoLib/include/Math/float3.h"
#include "Assimp/include/vector3.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "Texture.h"
#include <vector>
typedef unsigned int uint;
struct aiMesh;
struct aiScene;


class AssetMesh
{
public:
	~AssetMesh();
	bool LoadTexture(aiMesh * info,const aiScene* fbx, std::vector<Texture*>& textures);
	bool LoadVertices(const int num_vertices, const float * vertices);
	bool LoadVerticesNormals(aiMesh * info);
	bool LoadFaces(aiMesh* info);
	bool CalculateFaceNormals();
	bool LoadFaces(const int num_faces, const uint16_t * indices);
	bool LoadUVs(aiMesh * info);
	void CreateBoindingBox();
	bool GeneteVertexNormalsBuffer();
	bool GenerateVerticesBuffer();
	bool GenerateFacesAndNormalsBuffer();
	bool GenerateUVsBuffer();
	void CleanUp();
	AABB GetBondingBox()
	{
		return default_bonding_box;
	}
private:
	AABB default_bonding_box;
public:
	//Data ========================
	uint id_indice = 0u; // index in VRAM
	uint num_indices = 0u;
	uint * indices = nullptr;
	//TODO: Change to uint *
	//Assimp uses uint * and par_shapes uses uint16_t *

	//Vertec ================================
	uint id_vertex = 0u; // unique vertex in VRAM
	uint num_vertices = 0u;
	float3* vertices = nullptr;

	//Normals ===================
	float3* vertex_normals = nullptr;
	uint id_vertex_normals = 0u;
	uint num_faces = 0u;

	//TODO: We don't need float 3 pointer, we can do it with float, as the vertices
	float3* faces_normals = nullptr;
	float3* face_middle_point = nullptr;

	// UV ==================
	float* UVCoord = nullptr;
	uint id_uv = 0u;
	uint uv_num_components = 0u;
	uint id_texture = 0u;

	
};


#endif