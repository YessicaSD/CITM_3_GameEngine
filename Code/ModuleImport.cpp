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

#include "AssetMeshNode.h"

#include "ModuleFileSystem.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par\par_shapes.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

void AssimpWrite(const char *text, char *data)
{

	std::string tmp_txt = text;
	std::string::const_iterator index = std::remove(tmp_txt.begin(), tmp_txt.end(), '%');
	tmp_txt.erase(index, tmp_txt.end());
	LOG(tmp_txt.c_str());
}

ModuleImport::ModuleImport(const char *name) : Module(true, name)
{
}

bool ModuleImport::Start(JSONFile * config)
{
	LOG("Creating assimp LOG stream");
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = AssimpWrite;
	aiAttachLogStream(&stream);
	return true;
}

//Creates an AssetMesh (our custom format for 3d meshes) from an fbx
bool ModuleImport::ImportMesh(const char *path)
{
	const aiScene *scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		std::vector<AssetMesh *> object_meshes;
		std::vector<AssetTexture *> textures;//TODO: Clear this vectors
		//Texture list should be inside the AssetMesh too?
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{

			aiMesh *assimp_mesh = scene->mMeshes[i];
			AssetMesh *asset_mesh = LoadAssimpMesh(assimp_mesh);
			LoadFBXTexture(assimp_mesh, scene, textures);

			object_meshes.push_back(asset_mesh);
			meshes.push_back(asset_mesh);
		}
		//Get the nodes
		AssetMeshNode asset_mesh_node;
		CreateGameObjectsFromNodes(scene->mRootNode, App->scene->root_gameobject->transform, object_meshes, textures);
		
		//Save the file here
		//It has
		//- Meshes
			//- Indices
			//- Vertices
		//- Textures
		//- Nodes
		//There is no class encapsulating all of this
		//It's just a series of vectors that exist on this function

		//TODO: We should save information about children so we can release the scene here and only use the assetmesh to creat all the gameobject 
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}

	return true;
}

bool ModuleImport::LoadFBXTexture(aiMesh * info, const  aiScene* fbx, std::vector<AssetTexture*>& textures)
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

AssetMesh *ModuleImport::LoadAssimpMesh(aiMesh *assimp_mesh)
{
	AssetMesh *asset_mesh = new AssetMesh();
	//INFO: We can only do this cast because we know that aiVector3D is 3 consecutive floats
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

AssetMesh *ModuleImport::LoadParShapeMesh(par_shapes_mesh *mesh)
{
	AssetMesh *asset_mesh = new AssetMesh();

	asset_mesh->LoadVertices(mesh->npoints, mesh->points);
	asset_mesh->CreateBoundingBox();

	//TODO: Get vertices normals
	asset_mesh->LoadFaces(mesh->ntriangles, mesh->triangles);
	asset_mesh->CalculateFaceNormals();
	asset_mesh->LoadUVs(mesh->tcoords);

	asset_mesh->GenerateVerticesBuffer();
	asset_mesh->GenerateFacesAndNormalsBuffer();
	asset_mesh->GenerateUVsBuffer();

	return asset_mesh;
}

void ModuleImport::CreateGameObjectsFromNodes(aiNode *node, ComponentTransform *parent, std::vector<AssetMesh *> loaded_meshes, std::vector<AssetTexture *> &textures)
{
	GameObject *new_gameobject = new GameObject(std::string(node->mName.C_Str()), parent);

	aiVector3D translation, scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	new_gameobject->transform->SetTransform(float3(translation.x, translation.y, translation.z), float3(scaling.x, scaling.y, scaling.z), Quat(rotation.x, rotation.y, rotation.z, rotation.w));
	new_gameobject->transform->Reset();

	if (node->mNumMeshes > 0u)
	{
		//Load the meshes of this GameObject
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			int index = node->mMeshes[i];
			ComponentMesh *component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			component_mesh->mesh = loaded_meshes[index];
			component_mesh->bounding_box->MultiplyByMatrix(new_gameobject->transform->GetGlobalMatrix(), component_mesh->mesh->GetAABB());
			if (textures[index] != nullptr)
			{
				component_mesh->material->SetTexture(textures[index]);
			}
		}
	}

	for (int i = 0; i < node->mNumChildren; ++i)
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

void ModuleImport::EventRequest(const Event &event)
{
	if (event.type == Event::EVENT_TYPE::DROPPED_FILE)
	{

		std::string extension;
		App->file_system->GetExtension(event.path, extension);
		if (extension == "fbx" || extension == "FBX")
		{
			//Import mesh onto assets folder
			ImportMesh(event.path);
		}
		else if (extension == "dds" || extension == "png" || extension == "jpg")
		{
			//Import texture onto assets folder
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

GameObject *ModuleImport::CreateGameObjectWithMesh(std::string name, ComponentTransform *parent, AssetMesh *asset_mesh)
{
	GameObject *new_gameobject = new GameObject(name, parent);
	ComponentMesh *component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
	component_mesh->mesh = asset_mesh;
	new_gameobject->transform->UpdateDisplayValues();
	component_mesh->UpdateBoundingBox(new_gameobject->transform->GetGlobalMatrix());
	return new_gameobject;
}

bool ModuleImport::AddMesh(AssetMesh *mesh)
{
	meshes.push_back(mesh);
	return true;
}