#include "ResourceMesh.h"
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
#include "ResourceTexture.h"
#include "BoundingBox.h"
#include "ModuleFileSystem.h"

//TODO: Remove, only for testing purposes
#include "ModuleResourceManager.h"

ResourceMesh::ResourceMesh() : Resource()
{
}

ResourceMesh::~ResourceMesh()
{
	CleanUp();
}

//Generates and saves data to a file
bool ResourceMesh::SaveFileData()
{
	bool ret = false;

	//Generate file data
	uint ranges[] = {
		num_vertices,
		num_indices,
		uv_num_components };

	uint ranges_bytes = sizeof(ranges);
	uint vertices_bytes = sizeof(float3) * num_vertices;
	uint indices_bytes = sizeof(uint) * num_indices;
	uint uv_bytes = sizeof(float) * uv_num_components;

	uint size = ranges_bytes
		+ vertices_bytes
		+ indices_bytes
		+ uv_bytes;

	// Allocate
	char* data = new char[size]; 

	char* cursor = data;

	 //Store ranges in which it must read.
	memcpy(cursor, ranges, ranges_bytes);
	cursor += ranges_bytes;

	//Store vertices
	memcpy(cursor, vertices, vertices_bytes);
	cursor += vertices_bytes;

	//Store indices
	memcpy(cursor, indices, indices_bytes);
	cursor += indices_bytes;

	//Store uvs
	memcpy(cursor, uv_coord, uv_bytes);
	cursor += uv_bytes;

	//SaveFile
	uint path_size = 250u;
	char * path = new char[path_size];
	App->file_system->CreatePath(RESOURCES_MESH_FOLDER, "mesh", uid, "hinata_mesh", path, path_size);
	ret = App->file_system->SaveFile((const void *)data, size, &path);

	//TODO: Remove, only for testing purposes
	char * new_data = nullptr;
	App->file_system->LoadFile(path, &new_data);
	ResourceMesh * new_mesh = App->resource_manager->CreateNewResource<ResourceMesh>();
	new_mesh->LoadFileData(new_data);

	return ret;
}

bool ResourceMesh::LoadFileData(char * data)
{
	char * cursor = data;

	//INFO: The number of elements on the ranges array must be the same as in the ranges array of ResourceMesh::GenerateFileData()
	uint ranges[3];

	uint ranges_bytes = sizeof(ranges);
	uint vertices_bytes = sizeof(float3) * num_vertices;
	uint indices_bytes = sizeof(uint) * num_indices;
	uint uv_bytes = sizeof(float) * uv_num_components;

	memcpy(ranges, cursor, ranges_bytes);

	//Load ranges
	num_vertices = ranges[0];
	num_indices = ranges[1];
	uv_num_components = ranges[2];
	cursor += ranges_bytes;

	// Load vertices
	vertices = new float3[num_vertices];
	memcpy(vertices, cursor, vertices_bytes);
	cursor += vertices_bytes;

	//Load indices
	indices = new uint[num_indices];
	memcpy(indices, cursor, indices_bytes);
	cursor += indices_bytes;

	//Load uvs
	uv_coord = new float[uv_num_components];
	memcpy(uv_coord, cursor, uv_bytes);
	cursor += uv_bytes;

	return true;
}

bool ResourceMesh::LoadVertices(const int num_vertices, const float * vertices)
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

bool ResourceMesh::LoadVerticesNormals(aiMesh * info)
{
	if (info->HasNormals())
	{
		vertex_normals = new float3[info->mNumVertices];
		memcpy(vertex_normals, info->mNormals, sizeof(aiVector3D) * num_vertices);
	}
	return true;
}

bool ResourceMesh::GenerateVertexNormalsBuffer()
{
	if (vertex_normals != nullptr)
	{
		glGenBuffers(1, &id_vertex_normals);
		glBindBuffer(GL_ARRAY_BUFFER, this->id_vertex_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * this->num_vertices, vertex_normals, GL_STATIC_DRAW);
	}

	return true;
}

bool ResourceMesh::GenerateVerticesBuffer()
{
	if (vertices != nullptr)
	{
		glGenBuffers(1, &id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	}
	
	return true;
}

bool ResourceMesh::LoadFaces(aiMesh * info)
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

bool ResourceMesh::LoadFaces(const int num_faces, const uint * indices)
{
	this->num_faces = num_faces;
	num_indices = num_faces * 3;
	this->indices = new uint[num_indices];
	memcpy(this->indices, indices, sizeof(uint) * num_indices);
	
	return true;
}

bool ResourceMesh::CalculateFaceNormals()
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

bool ResourceMesh::LoadUVs(aiMesh * info)
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

bool ResourceMesh::LoadUVs(float * coords)
{
	if (coords)
	{
		this->uv_num_components = 2u;
		this->uv_coord = new float[uv_num_components * this->num_vertices];
		memcpy(this->uv_coord, coords, sizeof(float) * uv_num_components * this->num_vertices);

	}

	return true;
}

void ResourceMesh::CreateBoundingBox()
{
	aabb.Enclose(this->vertices, num_vertices);
}

bool ResourceMesh::GenerateFacesAndNormalsBuffer()
{
	if (num_faces > 0u)
	{
		glGenBuffers(1, &id_indice);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indice);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* num_indices, indices, GL_STATIC_DRAW);
	}
	return true;
}

bool ResourceMesh::GenerateUVsBuffer()
{
	if (uv_coord > 0)
	{
		glGenBuffers(1, &id_uv);
		glBindBuffer(GL_ARRAY_BUFFER, id_uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_num_components * num_vertices, &uv_coord[0], GL_STATIC_DRAW);
	}

	return true;
}

void ResourceMesh::CleanUp()
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

AABB ResourceMesh::GetAABB()
{
	return aabb;
}

