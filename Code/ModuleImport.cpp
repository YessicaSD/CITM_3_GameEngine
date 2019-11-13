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
#include "ResourceMesh.h"
#include "ModuleTexture.h"
#include "ModuleResourceManager.h"
#include "Event.h"

#include "Resource.h"
#include "ResourceModel.h"

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
bool ModuleImport::ImportModel(const char *path)
{
	const aiScene *scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		std::vector<ResourceMesh *> fbx_meshes;
		std::vector<UID> fbx_meshes_uids;

		std::vector<ResourceTexture *> fbx_textures;
		std::vector<UID> fbx_textures_uids;

		ResourceModelNode model_root_node;

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh *assimp_mesh = scene->mMeshes[i];
			ResourceMesh *resource_mesh = ImportAssimpMesh(assimp_mesh);
			fbx_meshes.push_back(resource_mesh);
			fbx_meshes_uids.push_back(resource_mesh->GetUID());
			meshes.push_back(resource_mesh);

			ResourceTexture * resource_texture = ImportFBXTexture(assimp_mesh, scene);
			fbx_textures.push_back(resource_texture);
			fbx_textures_uids.push_back(resource_texture->GetUID());
			//TODO: Push this texture on the texture array or in the resources array
		}

		LoadFBXNodes(&model_root_node, scene->mRootNode, fbx_meshes_uids, fbx_textures_uids);
		
		//SaveModel(object_meshes, textures, model_root_node);

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

//Saves the model in our own custom file format
//bool ModuleImport::SaveModel(const std::vector<ResourceMesh*> & meshes, const std::vector<ResourceTexture*> & textures)
//{
//	//1. Meshes
//		//indices
//		//vertices
//		//uvs
//	//2. Textures
//	//3. Nodes
//		//meshes id
//		//textures id
//
//
//}

bool ModuleImport::LoadFBXNodes(ResourceModelNode * model_node, aiNode * node, const std::vector<UID>& meshes, const std::vector<UID>& materials)
{
	model_node->name = (char*)node->mName.C_Str();
	model_node->transform = reinterpret_cast<const float4x4&>(node->mTransformation);

	if (node->mNumMeshes > 0u)
	{
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			uint index = node->mMeshes[i];
			model_node->mesh = index;
			model_node->material = index;
		}
		//Save parent id
		//How if it's recursive?
		//TODO: Make a push back here?
		//Tree only needs to be reconstructed to assemble GameObjects
		//And make a preview model

		//Do the same for the children
		for (int i = 0; i < node->mNumChildren; ++i)
		{
			LoadFBXNodes(new ResourceModelNode(), node->mChildren[i], meshes, materials);
		}
	}

	return true;
}

ResourceTexture * ModuleImport::ImportFBXTexture(aiMesh * info, const  aiScene* fbx)
{
	aiMaterial* material = fbx->mMaterials[info->mMaterialIndex];

	ResourceTexture * ret = nullptr;

	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString aipath;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aipath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string path = ASSETS_FOLDER + std::string(aipath.data);
			ret = App->texture->LoadTexture(path.c_str());
		}
		else
		{
			LOG("Texture path note found");
		}
	}
	return ret;
}

ResourceMesh *ModuleImport::ImportAssimpMesh(aiMesh *assimp_mesh)
{
	ResourceMesh *resource_mesh = App->resource_manager->CreateNewResource<ResourceMesh>();
	//INFO: We can only do this cast because we know that aiVector3D is 3 consecutive floats
	resource_mesh->LoadVertices(assimp_mesh->mNumVertices, (const float *)assimp_mesh->mVertices);
	resource_mesh->CreateBoundingBox();
	resource_mesh->LoadVerticesNormals(assimp_mesh);
	resource_mesh->LoadFaces(assimp_mesh);
	resource_mesh->CalculateFaceNormals();
	resource_mesh->LoadUVs(assimp_mesh);

	resource_mesh->GenerateVertexNormalsBuffer();
	resource_mesh->GenerateVerticesBuffer();
	resource_mesh->GenerateFacesAndNormalsBuffer();
	resource_mesh->GenerateUVsBuffer();

	resource_mesh->SaveFileData();

	return resource_mesh;
}

ResourceMesh *ModuleImport::LoadParShapeMesh(par_shapes_mesh *mesh)
{
	ResourceMesh *resource_mesh = App->resource_manager->CreateNewResource<ResourceMesh>();

	resource_mesh->LoadVertices(mesh->npoints, mesh->points);
	resource_mesh->CreateBoundingBox();

	//TODO: Get vertices normals
	resource_mesh->LoadFaces(mesh->ntriangles, mesh->triangles);
	resource_mesh->CalculateFaceNormals();
	resource_mesh->LoadUVs(mesh->tcoords);

	resource_mesh->GenerateVerticesBuffer();
	resource_mesh->GenerateFacesAndNormalsBuffer();
	resource_mesh->GenerateUVsBuffer();

	return resource_mesh;
}

void ModuleImport::CreateGameObjectsFromNodes(aiNode *node, ComponentTransform *parent, std::vector<ResourceMesh *> loaded_meshes, std::vector<ResourceTexture *> &textures)
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
			ImportModel(event.path);
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

GameObject *ModuleImport::CreateGameObjectWithMesh(std::string name, ComponentTransform *parent, ResourceMesh *asset_mesh)
{
	GameObject *new_gameobject = new GameObject(name, parent);
	ComponentMesh *component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
	component_mesh->mesh = asset_mesh;
	new_gameobject->transform->UpdateDisplayValues();
	component_mesh->UpdateBoundingBox(new_gameobject->transform->GetGlobalMatrix());
	return new_gameobject;
}

bool ModuleImport::AddMesh(ResourceMesh *mesh)
{
	meshes.push_back(mesh);
	return true;
}