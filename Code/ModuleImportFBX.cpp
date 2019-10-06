#include "ModuleImportFBX.h"


#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "Mesh.h"
//
//#define PAR_SHAPES_IMPLEMENTATION
//#include "par\par_shapes.h"
bool ModuleImportFBX::Start()
{
	
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	//LoadMesh("C:\Users\Yessica\Documents\GitHub\CITM_3_GameEngine\Code\Engine\warrior.FBX");
	return true;
}

bool ModuleImportFBX::LoadMesh(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		Scene* new_scene = new Scene();
		array_scene.push_back(new_scene);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* actual_mesh = scene->mMeshes[i];
			Mesh* new_mesh  = new Mesh();
			
			
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

				glGenBuffers(1, &new_mesh->id_indice);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indice);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*actual_mesh->mNumFaces * 3, new_mesh->indices, GL_STATIC_DRAW);

			}

			

			

			new_scene->array_mesh.push_back(new_mesh);
		}
		
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	
	return true;
}

bool ModuleImportFBX::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

update_status ModuleImportFBX::PostUpdate()
{
	for (std::vector<Scene*>::iterator scene_iter = array_scene.begin(); scene_iter != array_scene.end(); ++scene_iter)
	{
		for (std::vector<Mesh*>::iterator iter = (*scene_iter)->array_mesh.begin(); iter != (*scene_iter)->array_mesh.end(); ++iter)
		{
			if ((*iter))
			{
				
				(*iter)->Draw();
				(*iter)->DrawVertexNormal();
			}
		}
	}

	return UPDATE_CONTINUE;
}
