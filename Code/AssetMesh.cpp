#include "AssetMesh.h"
#include "Globals.h"
#include "glew/include/GL/glew.h"

void AssetMesh::LoadVertices(const uint num_vertices, float * vertices)
{
	this->num_vertices = num_vertices;
	this->vertices = new float[num_vertices * 3];
	memcpy(this->vertices, vertices, sizeof(float) * this->num_vertices * 3);
	LOG("New mesh with %d vertices", num_vertices);

	//TODO: Do we really need to store the vertices?
	//Or can we just store the buffer?

	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->num_vertices * 3, this->vertices, GL_STATIC_DRAW);
}

void AssetMesh::LoadFaces()
{
	//This code shouldn't depend on you passing a assimp_mesh, you could pass something else
	//if (assimp_mesh->HasNormals())
	//{
	//	asset_mesh->normals = new aiVector3D[assimp_mesh->mNumVertices];
	//	memcpy(asset_mesh->normals, assimp_mesh->mNormals, sizeof(aiVector3D) * assimp_mesh->mNumVertices);
	//}


	//asset_mesh->num_indices = assimp_mesh->mNumFaces * 3;
	//asset_mesh->indices = new uint[asset_mesh->num_indices]; // assume each face is a triangle
	//for (uint i = 0; i < assimp_mesh->mNumFaces; ++i)
	//{
	//	if (assimp_mesh->mFaces[i].mNumIndices != 3)
	//	{
	//		LOG("WARNING, geometry face with != 3 indices!");
	//	}
	//	else
	//	{
	//		memcpy(&asset_mesh->indices[i * 3], assimp_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
	//	}
	//}

	//asset_mesh->numFaces = assimp_mesh->mNumFaces;
	//asset_mesh->faces_normals = new float3[assimp_mesh->mNumFaces];
	//asset_mesh->face_middle_point = new float3[assimp_mesh->mNumFaces];

	//for (uint i = 0; i < asset_mesh->num_indices; i += 3)
	//{
	//	uint index = asset_mesh->indices[i];
	//	float3 vertex1 = { asset_mesh->vertices[index * 3], asset_mesh->vertices[index * 3 + 1] , asset_mesh->vertices[index * 3 + 2] };
	//	index = asset_mesh->indices[i + 1];

	//	float3 vertex2 = { asset_mesh->vertices[index * 3], asset_mesh->vertices[index * 3 + 1] , asset_mesh->vertices[index * 3 + 2] };
	//	index = asset_mesh->indices[i + 2];
	//	float3 vertex3 = { asset_mesh->vertices[index * 3], asset_mesh->vertices[index * 3 + 1] , asset_mesh->vertices[index * 3 + 2] };

	//	float3 vector1 = vertex2 - vertex1;
	//	float3 vector2 = vertex3 - vertex1;

	//	asset_mesh->faces_normals[i / 3] = Cross(vector1, vector2);
	//	asset_mesh->faces_normals[i / 3].Normalize();
	//	asset_mesh->face_middle_point[i / 3] = { (vertex1.x + vertex2.x + vertex3.x) / 3, (vertex1.y + vertex2.y + vertex3.y) / 3, (vertex1.z + vertex2.z + vertex3.z) / 3 };
	//}
	//glGenBuffers(1, &asset_mesh->id_indice);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, asset_mesh->id_indice);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*assimp_mesh->mNumFaces * 3, asset_mesh->indices, GL_STATIC_DRAW);
}