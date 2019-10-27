#include "Application.h"
#include "ModuleImport.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "AssetMesh.h"
#include "ModuleTexture.h"
#include "Event.h"

#include "ModuleFileSystem.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par\par_shapes.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

void AssimpWrite(const char * text, char * data)
{
	
	std::string tmp_txt = text;
	std::string::const_iterator index = std::remove(tmp_txt.begin(), tmp_txt.end(), '%');
	tmp_txt.erase(index, tmp_txt.end());
	LOG(tmp_txt.c_str());
}

ModuleImport::ModuleImport(const char * name) : Module(true, name)
{}

bool ModuleImport::Start(JSON_Object* config)
{
	LOG("Creating assimp LOG stream");
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = AssimpWrite;
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleImport::LoadMesh(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		std::vector<AssetMesh*> object_meshes;
		std::vector<Texture*> textures;
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			
			aiMesh* assimp_mesh = scene->mMeshes[i];
			AssetMesh * asset_mesh = LoadAssimpMesh(assimp_mesh, scene, textures);
				
			object_meshes.push_back(asset_mesh);
			meshes.push_back(asset_mesh);
		}
		CreateGameObjectsFromNodes(scene->mRootNode, App->scene->root_gameobject->transform, object_meshes, textures);
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}
	
	return true;
}

AssetMesh * ModuleImport::LoadAssimpMesh(aiMesh * assimp_mesh, const aiScene* scene_fbx, std::vector<Texture*>& textures)
{
	AssetMesh * asset_mesh = new AssetMesh();
	//INFO: We can only do this cast because we know that aiVector3D is 3 consecutive floats
	asset_mesh->LoadTexture(assimp_mesh, scene_fbx, textures);
	asset_mesh->LoadVertices(assimp_mesh->mNumVertices, (const float *)assimp_mesh->mVertices);
	asset_mesh->CreateBoundingBox();
	asset_mesh->LoadVerticesNormals(assimp_mesh);
	asset_mesh->LoadFaces(assimp_mesh);
	asset_mesh->CalculateFaceNormals();
	asset_mesh->LoadUVs(assimp_mesh);

	asset_mesh->GenerateVertexNormalsBuffer();
	asset_mesh->GenerateVerticesBuffer();
	asset_mesh->GenerateFacesAndNormalsBuffer();
	asset_mesh->GenerateUVsBuffer();
	return asset_mesh;
}

AssetMesh* ModuleImport::LoadParShapeMesh(par_shapes_mesh * mesh)
{
	AssetMesh * asset_mesh = new AssetMesh();

	asset_mesh->LoadVertices(mesh->npoints, mesh->points);
	asset_mesh->CreateBoundingBox();
	
	//TODO: Get vertices normals
	asset_mesh->LoadFaces(mesh->ntriangles, mesh->triangles);
	asset_mesh->CalculateFaceNormals();
	asset_mesh->LoadUVs(mesh->tcoords);

	asset_mesh->GenerateVerticesBuffer();
	asset_mesh->GenerateFacesAndNormalsBuffer();
	asset_mesh->GenerateUVsBuffer();

	App->import->meshes.push_back(asset_mesh);
	return asset_mesh;
}

void ModuleImport::CreateGameObjectsFromNodes(aiNode * node, ComponentTransform * parent, std::vector<AssetMesh*> loaded_meshes, std::vector<Texture*>& textures)
{
	GameObject * new_gameobject = new GameObject(std::string(node->mName.C_Str()), parent);
	
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	new_gameobject->transform->CalculGlobalMatrix(float3(translation.x, translation.y, translation.z), float3(scaling.x, scaling.y, scaling.z), Quat(rotation.x, rotation.y, rotation.z, rotation.w));
	
	//TODO: Search if there is a better way to convert from aiMatrix4x4 to math::float4x4 (both are arrays with 16 positions at the end)
	//V1
	/*new_gameobject->transform->local_matrix.Set(
		node->mTransformation.a1, node->mTransformation.b1, node->mTransformation.c1, node->mTransformation.d1,
		node->mTransformation.a2, node->mTransformation.b2, node->mTransformation.c2, node->mTransformation.d2,
		node->mTransformation.a3, node->mTransformation.b3, node->mTransformation.c3, node->mTransformation.d3,
		node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4, node->mTransformation.d4);*/
	//memcpy(&new_gameobject->transform.local_matrix, &node->mTransformation, sizeof(float) * 16);
	//V2
	//new_gameobject->transform.local_matrix.Set(
	//	node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4,
	//	node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4,
	//	node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4,
	//	node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4);

	//TODO: Calculate global matrix after that, don't set it directly to the local matrix of the fbx node
	//V1
	//new_gameobject->transform->UpdatePos();

	//V2
	//new_gameobject->transform.global_matrix = parent->global_matrix * new_gameobject->transform.local_matrix;

	if (node->mNumMeshes > 0u)
	{
		//Load the meshes of this GameObject
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			int index = node->mMeshes[i];
			ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			component_mesh->mesh = loaded_meshes[index];
			if (textures[index])
			{
				component_mesh->material->SetTexture(textures[index]);
			}
			
		}
	}

	for (int i = 0 ; i < node->mNumChildren; ++i)
	{
		CreateGameObjectsFromNodes(node->mChildren[i], new_gameobject->transform, loaded_meshes, textures);
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
		if (extension == "fbx" || extension == "FBX")
		{
			LoadMesh(event.path);
		}
		else if (extension == "dds" || extension == "png" || extension == "jpg")
		{
			App->texture->LoadTexture(event.path);
		}
		else
		{
			LOG("Can't load this type file");
			return;
		}
		LOG("File dropped %s", event.path);
	}
}

void ModuleImport::CreateGameObjectWithMesh(std::string name, ComponentTransform * parent, AssetMesh * asset_mesh)
{
	GameObject * new_gameobject = new GameObject(name, parent);
	ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
	component_mesh->mesh = asset_mesh;
	new_gameobject->transform->CalculPRSWithMatrix();
}