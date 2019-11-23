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
	RESOURCE_DECLARATION(ResourceMesh);

private:
	ResourceMesh();//Constructor is private because Resources need to be created from ModuleResourceManager

	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

public:
	~ResourceMesh();

	bool ImportVertices(const int num_vertices, const float *vertices);
	bool ImportVerticesNormals(aiMesh *info);
	bool ImportFaces(aiMesh *info);
	bool ImportFaces(const int num_faces, const uint *indices);
	bool CalculateFaceNormals();
	bool ImportUVs(aiMesh *info);
	bool ImportUVs(float *coods);

	bool GenerateVerticesBuffer();
	bool GenerateVertexNormalsBuffer();
	bool GenerateFacesBuffer();
	bool GenerateFaceNormalsBuffer();
	bool GenerateUVsBuffer();

	void CreateBoundingBox();
	void CleanUp();
	AABB GetAABB();
	uint GetUVCoordSize();

private:
	AABB aabb;

public:
	//Vertices
	uint id_vertex = 0u; // unique vertex in VRAM
	uint num_vertices = 0u;
	float3 *vertices = nullptr;

	//Vertices normals
	float3 *vertex_normals = nullptr;
	uint id_vertex_normals = 0u;

	//Faces / Indices
	uint id_indice = 0u; // index in VRAM
	uint num_indices = 0u;
	uint num_faces = 0u;
	uint *indices = nullptr;

	//Faces normals
	//TODO: We don't need float 3 pointer, we can do it with float, as the vertices
	float3 *faces_normals = nullptr;
	float3 *face_middle_point = nullptr;

	//UV coordinates
	float *uv_coord = nullptr;
	uint id_uv = 0u;
	uint uv_dimensions = 0u;//Whether the UVs are stored as 2D or 3D

	friend class ModuleImport;
	friend class ModuleResourceManager;
};

#endif