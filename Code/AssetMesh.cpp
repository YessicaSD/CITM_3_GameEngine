#include "AssetMesh.h"
#include "Globals.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>
#include "Application.h"
#include "ModuleScene.h"
#include "Assimp/include/mesh.h"
#include "ModuleImport.h"
#include "Texture.h"



AssetMesh::~AssetMesh()
{
	CleanUp();
}

bool AssetMesh::LoadVertices(const int num_vertices, const float * vertices)
{
	this->num_vertices = num_vertices;
	this->vertices = new float[num_vertices * 3];
	memcpy(this->vertices, vertices, sizeof(float) * num_vertices * 3);
	return true;
}

bool AssetMesh::LoadVerticesNormals(aiMesh * info)
{
	if (info->HasNormals())
	{
		normals = new float3[info->mNumVertices];
		memcpy(normals, info->mNormals, sizeof(aiVector3D) * num_vertices);
	}
	return true;
}

bool AssetMesh::GenerateVerticesBuffer()
{
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
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

bool AssetMesh::CalculateFaceNormals()
{
	faces_normals = new float3[num_faces];
	face_middle_point = new float3[num_faces];
	uint curr_vertex = 0u;
	for (uint i = 0u; i < num_indices; i += 3u)
	{
		uint index = indices[i];
		curr_vertex = index * 3;
		float3 vertex1 = { vertices[curr_vertex], vertices[curr_vertex + 1] , vertices[curr_vertex + 2] };

		index = indices[i + 1];
		curr_vertex = index * 3;
		float3 vertex2 = { vertices[curr_vertex], vertices[curr_vertex + 1] , vertices[curr_vertex + 2] };

		index = indices[i + 2];
		curr_vertex = index * 3;
		float3 vertex3 = { vertices[curr_vertex], vertices[curr_vertex + 1] , vertices[curr_vertex + 2] };

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
			UVCoord = new float[num_vertices*uv_num_components];
			for (uint i = 0u; i < num_vertices; ++i)
			{
				memcpy(&UVCoord[i*uv_num_components], &info->mTextureCoords[0][i], sizeof(float)* uv_num_components);
			}
		}
	}

	return true;
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
	if (UVCoord > 0)
	{
		glGenBuffers(1, &id_uv);
		glBindBuffer(GL_ARRAY_BUFFER, id_uv);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_num_components * num_vertices, &UVCoord[0], GL_STATIC_DRAW);
	}
	return true;
}

void AssetMesh::CleanUp()
{
	if (indices)
	{
		delete indices;
		indices = nullptr;
	}
	if (vertices)
	{
		delete vertices;
		vertices = nullptr;
	}
	if (normals)
	{
		delete normals;
		normals = nullptr;
	}
	if (faces_normals)
	{
		delete faces_normals;
		faces_normals = nullptr;
	}
	if (face_middle_point)
	{
		delete face_middle_point;
		face_middle_point = nullptr;
	}
	if (UVCoord)
	{
		delete UVCoord;
		UVCoord = nullptr;
	}
}


//INFO: We could save some memory by keeping index as uint16_t values instead of uint
//But we would need to create another class for AssetMesh with uint16_t or use templates with specialization
//We should change:
//- The indices type
//- In ComponentMesh::OnPostUpdate() the parameter type in DrawElements() from GL_UNSIGNED_INT to GL_UNSIGNED_SHORT
bool AssetMesh::LoadFaces(const int num_faces, const uint16_t * indices)
{
	this->num_faces = num_faces;
	this->num_indices = num_faces * 3;
	this->indices = new uint[num_indices];

	for (int i = 0; i < num_indices; ++i)
	{
		this->indices[i] = (uint)indices[i];
	}
	//TODO: Generate normals for par_shapes.h shapes

	return true;
}