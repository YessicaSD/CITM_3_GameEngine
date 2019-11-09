#include "AssetMesh.h"
#include "Globals.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>
#include "Application.h"
#include "ModuleScene.h"
#include "Assimp/include/mesh.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/material.h"
#include "ModuleImport.h"
#include "ModuleTexture.h"
#include "AssetTexture.h"
#include "BoundingBox.h"

AssetMesh::~AssetMesh()
{
	CleanUp();
}

bool AssetMesh::LoadTexture(aiMesh * info,const  aiScene* fbx, std::vector<AssetTexture*>& textures)
{
	aiMaterial* material = fbx->mMaterials[info->mMaterialIndex];
	
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString aipath;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aipath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string path = ASSETS_FOLDER + std::string(aipath.data);
			textures.push_back(App->texture->LoadTexture(path.c_str()));
			return true;
		}
		else
		{
			LOG("Texture path note found");
			textures.push_back(nullptr);
			return false;
		}
	}

	textures.push_back(nullptr);
	return false;
}

bool AssetMesh::LoadVertices(const int num_vertices, const float * vertices)
{
	this->num_vertices = num_vertices;
	this->vertices = new float3[num_vertices];

	//TODO change to memcopy
	uint indices_index = 0u;
	uint vertex_index = 0u;
	while (indices_index < num_vertices)
	{
		this->vertices[indices_index] = { vertices[vertex_index],vertices[vertex_index + 1u],vertices[vertex_index + 2u] };
		vertex_index += 3u;
		indices_index++;
	}

	//memcpy(this->vertices, vertices, sizeof(float) * num_vertices * 3);
	return true;
}

bool AssetMesh::LoadVerticesNormals(aiMesh * info)
{
	if (info->HasNormals())
	{
		vertex_normals = new float3[info->mNumVertices];
		memcpy(vertex_normals, info->mNormals, sizeof(aiVector3D) * num_vertices);
	}
	return true;
}

bool AssetMesh::GenerateVertexNormalsBuffer()
{
	if (vertex_normals)
	{
		glGenBuffers(1, &id_vertex_normals);
		glBindBuffer(GL_ARRAY_BUFFER, this->id_vertex_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * this->num_vertices, vertex_normals, GL_STATIC_DRAW);
	}

	return true;
}

bool AssetMesh::GenerateVerticesBuffer()
{
	if (vertices)
	{
		glGenBuffers(1, &id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	}
	
	return true;
}

bool AssetMesh::LoadFaces(aiMesh * info)
{
	if (info->HasFaces())
	{
		num_faces = info->mNumFaces;
		num_indices = info->mNumFaces * 3;
		indices = new uint[num_indices]; // assume each face is a triangle
		for (uint i = 0; i < info->mNumFaces; ++i)
		{
			if (info->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&indices[i * 3], info->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	return true;
}

bool AssetMesh::LoadFaces(const int num_faces, const uint * indices)
{
	this->num_faces = num_faces;
	num_indices = num_faces * 3;
	this->indices = new uint[num_indices];
	memcpy(this->indices, indices, sizeof(uint) * num_indices);
	
	return true;
}

bool AssetMesh::CalculateFaceNormals()
{
	faces_normals = new float3[num_faces];
	face_middle_point = new float3[num_faces];
	uint curr_vertex = 0u;
	for (uint i = 0u; i < num_indices; ++i)
	{
		uint index = indices[i];
		float3 vertex1 = vertices[curr_vertex];

		index = indices[i + 1u];
		float3 vertex2 = vertices[curr_vertex];

		index = indices[i + 2u];
		float3 vertex3 = vertices[curr_vertex];

		float3 vector1 = vertex2 - vertex1;
		float3 vector2 = vertex3 - vertex1;

		faces_normals[i / 3] = Cross(vector1, vector2);
		faces_normals[i / 3].Normalize();
		face_middle_point[i / 3] = { (vertex1.x + vertex2.x + vertex3.x) / 3, (vertex1.y + vertex2.y + vertex3.y) / 3, (vertex1.z + vertex2.z + vertex3.z) / 3 };

	}
	return true;
}

bool AssetMesh::LoadUVs(aiMesh * info)
{
	if (info->HasTextureCoords(0u))
	{
		//num components determines the dimention if the uv
		//the majority of them got only x and y but some uv got also has a z coordinate
		//for now one we are only going to load uv coordinates which have 2 dimensions
		uv_num_components = info->mNumUVComponents[0];
		if (uv_num_components == 2u)
		{
			uv_coord = new float[num_vertices*uv_num_components];
			for (uint i = 0u; i < num_vertices; ++i)
			{
				memcpy(&uv_coord[i*uv_num_components], &info->mTextureCoords[0][i], sizeof(float)* uv_num_components);
			}
		}
	}

	return true;
}

bool AssetMesh::LoadUVs(float * coords)
{
	if (coords)
	{
		this->uv_num_components = 2;
		this->uv_coord = new float[uv_num_components * this->num_vertices];
		memcpy(this->uv_coord, coords, sizeof(float) * uv_num_components * this->num_vertices);

	}

	return true;
}

void AssetMesh::CreateBoundingBox()
{
	aabb.Enclose(this->vertices, num_vertices);
}

bool AssetMesh::GenerateFacesAndNormalsBuffer()
{
	if (num_faces > 0)
	{
		glGenBuffers(1, &id_indice);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indice);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* num_indices, indices, GL_STATIC_DRAW);
	}
	return true;
}

bool AssetMesh::GenerateUVsBuffer()
{
	if (uv_coord > 0)
	{
		glGenBuffers(1, &id_uv);
		glBindBuffer(GL_ARRAY_BUFFER, id_uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_num_components * num_vertices, &uv_coord[0], GL_STATIC_DRAW);
	}

	return true;
}

void AssetMesh::CleanUp()
{
	if (indices)
	{
		delete[] indices;
		indices = nullptr;
	}
	if (vertices)
	{
		delete[] vertices;
		vertices = nullptr;
	}
	if (vertex_normals)
	{
		delete[] vertex_normals;
		vertex_normals = nullptr;
	}
	if (faces_normals)
	{
		delete[] faces_normals;
		faces_normals = nullptr;
	}
	if (face_middle_point)
	{
		delete[] face_middle_point;
		face_middle_point = nullptr;
	}
	if (uv_coord)
	{
		delete[] uv_coord;
		uv_coord = nullptr;
	}
}

AABB AssetMesh::GetAABB()
{
	return aabb;
}


