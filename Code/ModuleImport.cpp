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
#include "AssetMesh.h"
#include "ModuleTexture.h"
#include "Event.h"

#include "ModuleFileSystem.h"

bool ModuleImport::Start()
{
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	lenna_img_id = App->texture->LoadTexture("Assets/Baker_house.dds");
	return true;
}

bool ModuleImport::LoadMesh(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		AssimpScene* assimp_scene = new AssimpScene();
		assimp_scenes.push_back(assimp_scene);

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* assimp_mesh = scene->mMeshes[i];
			AssetMesh * asset_mesh = new AssetMesh();
			
			asset_mesh->LoadInfo(assimp_mesh);
			asset_mesh->GenereteBuffer(assimp_mesh);
			
			assimp_scene->assimp_meshes.push_back(asset_mesh);

		}

		//2 associate meshes
		CreateGameObjectsFromNodes(scene->mRootNode, &App->scene->root_gameobject.transform, assimp_scene);

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}
	
	return true;
}



void ModuleImport::CreateGameObjectsFromNodes(aiNode * node, ComponentTransform * parent, AssimpScene * assimp_scene)
{
	GameObject * new_gameobject = new GameObject(std::string(node->mName.C_Str()), parent);

	//TODO: Search if there is a better way to convert from aiMatrix4x4 to math::float4x4 (both are arrays with 16 positions at the end)
	//V1
	new_gameobject->transform.local_matrix.Set(
		node->mTransformation.a1, node->mTransformation.b1, node->mTransformation.c1, node->mTransformation.d1,
		node->mTransformation.a2, node->mTransformation.b2, node->mTransformation.c2, node->mTransformation.d2,
		node->mTransformation.a3, node->mTransformation.b3, node->mTransformation.c3, node->mTransformation.d3,
		node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4, node->mTransformation.d4);
	//memcpy(&new_gameobject->transform.local_matrix, &node->mTransformation, sizeof(float) * 16);
	//V2
	//new_gameobject->transform.local_matrix.Set(
	//	node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4,
	//	node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4,
	//	node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4,
	//	node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4);

	//TODO: Calculate global matrix after that, don't set it directly to the local matrix of the fbx node
	//V1
	new_gameobject->transform.global_matrix = new_gameobject->transform.local_matrix * parent->global_matrix;
	//V2
	//new_gameobject->transform.global_matrix = parent->global_matrix * new_gameobject->transform.local_matrix;

	if (node->mNumMeshes > 0u)
	{
		//Load the meshes of this GameObject
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			component_mesh->mesh = assimp_scene->assimp_meshes[node->mMeshes[i]];
		}
	}

	for (int i = 0 ; i < node->mNumChildren; ++i)
	{
		CreateGameObjectsFromNodes(node->mChildren[i], &new_gameobject->transform, assimp_scene);
	}
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

void ModuleImport::EventRequest(const Event & event)
{
	if (event.type == Event::EVENT_TYPE::DROPPED_FILE)
	{
		
		std::string extension;
		App->file_system->GetExtension(event.path, extension);
		if (extension == "fbx")
		{
			LoadMesh(event.path);

		}
		if (extension == "dds")
		{
			//Load texure 
		}
		else
		{
			return;
		}
		LOG("File dropped %s", event.path);
	}
}




