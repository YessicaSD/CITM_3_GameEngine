#include "AssetMesh.h"
#include "Globals.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>
#include "Application.h"
#include "ModuleScene.h"
#include "Assimp/include/mesh.h"
#include "ModuleImport.h"
#include "Texture.h"



bool AssetMesh::LoadInfo(aiMesh * info)
{
	//VERTEX INFO
	this->num_vertices = info->mNumVertices;
	this->vertices = new float[num_vertices * 3];
	memcpy(this->vertices, info->mVertices, sizeof(float) *num_vertices * 3);
	LOG("New mesh with %d vertices", num_vertices);

	//FACES INFO
	if (info->HasFaces())
	{
		numFaces = info->mNumFaces;
		num_indices = info->mNumFaces * 3;
		//indices = new uint[num_indices]; // assume each face is a triangle
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

		//FACE NORMALS
		faces_normals = new float3[numFaces];
		face_middle_point = new float3[numFaces];
		for (uint i = 0; i < num_indices; i += 3)
		{
			uint index = indices[i];
			float3 vertex1 = { vertices[index * 3], vertices[index * 3 + 1] , vertices[index * 3 + 2] };
			index = indices[i + 1];

			float3 vertex2 = { vertices[index * 3], vertices[index * 3 + 1] , vertices[index * 3 + 2] };
			index = indices[i + 2];
			float3 vertex3 = { vertices[index * 3], vertices[index * 3 + 1] , vertices[index * 3 + 2] };

			float3 vector1 = vertex2 - vertex1;
			float3 vector2 = vertex3 - vertex1;

			faces_normals[i / 3] = Cross(vector1, vector2);
			faces_normals[i / 3].Normalize();
			face_middle_point[i / 3] = { (vertex1.x + vertex2.x + vertex3.x) / 3, (vertex1.y + vertex2.y + vertex3.y) / 3, (vertex1.z + vertex2.z + vertex3.z) / 3 };

		}
	}

	//NORMALS INFO
	if (info->HasNormals())
	{
		normals = new float3[info->mNumVertices];
		memcpy(normals, info->mNormals, sizeof(aiVector3D) * num_vertices);
	}

	if (info->HasTextureCoords(0))
	{
		//num components determines the dimention if the uv
		//the majority of them got only x and y but some uv got also the z coordenete
		//for now one we are only going to load uv coordenates that are 2 dimentionals
		uv_num_components = info->mNumUVComponents[0];
		if (uv_num_components == 2)
		{
			UVCoord = new float[num_vertices*uv_num_components];

			for (uint i = 0; i < num_vertices; ++i)
			{
				memcpy(&UVCoord[i*uv_num_components], &info->mTextureCoords[0][i], sizeof(float)* uv_num_components);

			}
		}
	}

	return true;
}

bool AssetMesh::GenereteBuffer(aiMesh * info)
{
	//GENERETE VERTEX BUFFER 
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);

	//INDEX BUFFERS
	if (numFaces > 0)
	{
		glGenBuffers(1, &id_indice);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indice);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* numFaces * 3, indices, GL_STATIC_DRAW);
	}

	//UV BUFFER
	if (UVCoord > 0)
	{
		glGenBuffers(1, &id_uv);
		glBindBuffer(GL_ARRAY_BUFFER, id_uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_num_components * num_vertices, &UVCoord[0], GL_STATIC_DRAW);
	}


	return true;
}
