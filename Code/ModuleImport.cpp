#include "Application.h"
#include "ModuleImport.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "GameObject.h"
#include "Mesh.h"

bool ModuleImport::Start()
{
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleImport::LoadMesh(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);


	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject * assimp_scene_gameobject = new GameObject(std::string(path), &App->scene->root_gameobject.transform);

		AssimpScene* new_scene = new AssimpScene();
		array_scene.push_back(new_scene);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* actual_mesh = scene->mMeshes[i];
			GameObject * mesh_gameobject = new GameObject(std::string(actual_mesh->mName.C_Str()), &assimp_scene_gameobject->transform);
			Mesh * new_mesh = mesh_gameobject->CreateComponent<Mesh>();

			new_mesh->num_vertices = actual_mesh->mNumVertices;
			new_mesh->vertices = new float[new_mesh->num_vertices * 3];
			memcpy(new_mesh->vertices, actual_mesh->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", new_mesh->num_vertices);

			glGenBuffers(1, &new_mesh->id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);

			// copy faces
			if (actual_mesh->HasFaces())
			{
				if (actual_mesh->HasNormals())
				{
					new_mesh->normals = new aiVector3D[actual_mesh->mNumVertices];
					memcpy(new_mesh->normals, actual_mesh->mNormals, sizeof(aiVector3D) * actual_mesh->mNumVertices);
				}
				
				
				new_mesh->num_indices = actual_mesh->mNumFaces * 3;
				new_mesh->indices = new uint[new_mesh->num_indices]; // assume each face is a triangle
				for (uint i = 0; i < actual_mesh->mNumFaces; ++i)
				{
					if (actual_mesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&new_mesh->indices[i * 3], actual_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				new_mesh->numFaces = actual_mesh->mNumFaces;
				new_mesh->faces_normals = new float3[actual_mesh->mNumFaces];
				new_mesh->face_middle_point = new float3[actual_mesh->mNumFaces];

				for (uint i = 0; i < new_mesh->num_indices; i+=3)
				{
					uint index = new_mesh->indices[i];
					float3 vertex1 = { new_mesh->vertices[index * 3], new_mesh->vertices[index * 3 + 1] , new_mesh->vertices[index * 3 + 2] };
					 index = new_mesh->indices[i+1];

					float3 vertex2 = { new_mesh->vertices[index * 3], new_mesh->vertices[index * 3 + 1] , new_mesh->vertices[index * 3 + 2]  };
					index = new_mesh->indices[i + 2];
					float3 vertex3 = { new_mesh->vertices[index * 3], new_mesh->vertices[index * 3 + 1] , new_mesh->vertices[index * 3 + 2]  };

					float3 vector1 = vertex2-vertex1;
					float3 vector2  = vertex3 - vertex1;

					new_mesh->faces_normals[i/3] = Cross(vector1, vector2);
					new_mesh->faces_normals[i / 3].Normalize();
					new_mesh->face_middle_point[i/3] = { (vertex1.x + vertex2.x + vertex3.x)/3, (vertex1.y + vertex2.y + vertex3.y) / 3, (vertex1.z + vertex2.z + vertex3.z) / 3 };

				}
				glGenBuffers(1, &new_mesh->id_indice);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indice);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*actual_mesh->mNumFaces * 3, new_mesh->indices, GL_STATIC_DRAW);
			}
			new_scene->assimp_meshes.push_back(new_mesh);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}
	
	return true;
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}




