#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "MathGeoLib/include/Math/float3.h"
#include "Assimp/include/vector3.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "Resource.h"
#include "ResourceTexture.h"
#include <vector>

typedef unsigned int uint;
struct aiMesh;
struct aiScene;

class ResourceMesh : public Resource
{
private:
	ResourceMesh();//Constructor is private because Resources need to be created from ModuleResourceManager
public:
	~ResourceMesh();

	bool SaveFileData() override;
	bool LoadFileData(char ** data) override;

	bool LoadVertices(const int num_vertices, const float *vertices);
	bool LoadVerticesNormals(aiMesh *info);
	bool LoadFaces(aiMesh *info);
	bool CalculateFaceNormals();
	bool LoadFaces(const int num_faces, const uint *indices);
	bool LoadUVs(aiMesh *info);
	bool LoadUVs(float *coods);
	bool GenerateVertexNormalsBuffer();
	void CreateBoundingBox();
	bool GenerateVerticesBuffer();
	bool GenerateFacesAndNormalsBuffer();
	bool GenerateUVsBuffer();
	void CleanUp();
	AABB GetAABB();

private:
	AABB aabb;

public:
	//Data ========================
	uint id_indice = 0u; // index in VRAM
	uint num_indices = 0u;
	uint *indices = nullptr;
	//TODO: Change to uint *
	//Assimp uses uint * and par_shapes uses uint16_t *

	//Vertec ================================
	uint id_vertex = 0u; // unique vertex in VRAM
	uint num_vertices = 0u;
	float3 *vertices = nullptr;

	//Normals ===================
	float3 *vertex_normals = nullptr;
	uint id_vertex_normals = 0u;
	uint num_faces = 0u;

	//TODO: We don't need float 3 pointer, we can do it with float, as the vertices
	float3 *faces_normals = nullptr;
	float3 *face_middle_point = nullptr;

	// UV ==================
	float *uv_coord = nullptr;
	uint id_uv = 0u;
	uint uv_num_components = 0u;
	uint id_texture = 0u;

	friend class ModuleResourceManager;
};

#endif