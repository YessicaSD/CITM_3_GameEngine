#include "Application.h"
#include "ModuleImport.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "GameObject.h"
#include "ComponentMesh.h"

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

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* assimp_mesh = scene->mMeshes[i];
			GameObject * mesh_gameobject = new GameObject(std::string(assimp_mesh->mName.C_Str()), &assimp_scene_gameobject->transform);
			ComponentMesh * mesh_component = mesh_gameobject->CreateComponent<ComponentMesh>();

			LoadVertices(mesh_component, assimp_mesh);

			// copy faces
			if (assimp_mesh->HasFaces())
			{
				if (assimp_mesh->HasNormals())
				{
					mesh_component->normals = new aiVector3D[assimp_mesh->mNumVertices];
					memcpy(mesh_component->normals, assimp_mesh->mNormals, sizeof(aiVector3D) * assimp_mesh->mNumVertices);
				}
				
				
				mesh_component->num_indices = assimp_mesh->mNumFaces * 3;
				mesh_component->indices = new uint[mesh_component->num_indices]; // assume each face is a triangle
				for (uint i = 0; i < assimp_mesh->mNumFaces; ++i)
				{
					if (assimp_mesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh_component->indices[i * 3], assimp_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				mesh_component->numFaces = assimp_mesh->mNumFaces;
				mesh_component->faces_normals = new float3[assimp_mesh->mNumFaces];
				mesh_component->face_middle_point = new float3[assimp_mesh->mNumFaces];

				for (uint i = 0; i < mesh_component->num_indices; i+=3)
				{
					uint index = mesh_component->indices[i];
					float3 vertex1 = { mesh_component->vertices[index * 3], mesh_component->vertices[index * 3 + 1] , mesh_component->vertices[index * 3 + 2] };
					 index = mesh_component->indices[i+1];

					float3 vertex2 = { mesh_component->vertices[index * 3], mesh_component->vertices[index * 3 + 1] , mesh_component->vertices[index * 3 + 2]  };
					index = mesh_component->indices[i + 2];
					float3 vertex3 = { mesh_component->vertices[index * 3], mesh_component->vertices[index * 3 + 1] , mesh_component->vertices[index * 3 + 2]  };

					float3 vector1 = vertex2-vertex1;
					float3 vector2  = vertex3 - vertex1;

					mesh_component->faces_normals[i/3] = Cross(vector1, vector2);
					mesh_component->faces_normals[i / 3].Normalize();
					mesh_component->face_middle_point[i/3] = { (vertex1.x + vertex2.x + vertex3.x)/3, (vertex1.y + vertex2.y + vertex3.y) / 3, (vertex1.z + vertex2.z + vertex3.z) / 3 };

				}
				glGenBuffers(1, &mesh_component->id_indice);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_component->id_indice);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*assimp_mesh->mNumFaces * 3, mesh_component->indices, GL_STATIC_DRAW);
			}
			new_scene->assimp_meshes.push_back(mesh_component);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}
	
	return true;
}

void ModuleImport::LoadVertices(ComponentMesh * new_mesh, aiMesh * actual_mesh)
{
	new_mesh->num_vertices = actual_mesh->mNumVertices;
	new_mesh->vertices = new float[new_mesh->num_vertices * 3];
	memcpy(new_mesh->vertices, actual_mesh->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
	LOG("New mesh with %d vertices", new_mesh->num_vertices);

	glGenBuffers(1, &new_mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}




