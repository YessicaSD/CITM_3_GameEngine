#include "Application.h"
#include "ModuleImportModel.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "ResourceAnimation.h"

#include "ModuleRenderer3D.h"
#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentAnimation.h"

#include "ModuleImportTexture.h"
#include "ModuleResourceManager.h"
#include "Event.h"
#include "ModuleFileSystem.h"
#include "PhysFS/include/physfs.h"
#include "ModuleImportMesh.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par\par_shapes.h"
#include "BoundingBox.h"
#include <sys/stat.h>

#pragma comment(lib, "Assimp/libx86/assimp.lib")

void AssimpWriteLogStream(const char *text, char *data)
{

	std::string tmp_txt = text;
	std::string::const_iterator index = std::remove(tmp_txt.begin(), tmp_txt.end(), '%');
	tmp_txt.erase(index, tmp_txt.end());
	LOG(tmp_txt.c_str());
}

ModuleImportModel::ModuleImportModel(const char *name) : Module(true, name)
{
}

bool ModuleImportModel::Start(JSONFile * config)
{
	LOG("Creating assimp LOG stream");
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = AssimpWriteLogStream;
	aiAttachLogStream(&stream);
	return true;
}

//INFO: Creates a .hinata_model (our custom format for 3d models) from an fbx
ResourceModel * ModuleImportModel::ImportModel(const char *asset_path, UID model_uid, std::vector<UID> & prev_meshes_uids, std::vector<UID> & prev_textures_uids, std::vector<UID>& animation_uids)
{
	Timer import_timer;
	unsigned flags = aiProcess_CalcTangentSpace | \
		aiProcess_GenSmoothNormals | \
		aiProcess_JoinIdenticalVertices | \
		aiProcess_ImproveCacheLocality | \
		aiProcess_LimitBoneWeights | \
		aiProcess_SplitLargeMeshes | \
		aiProcess_Triangulate | \
		aiProcess_GenUVCoords | \
		aiProcess_SortByPType | \
		aiProcess_FindDegenerates | \
		aiProcess_FindInvalidData | \
		aiProcess_FlipUVs | \
		0;
	ResourceModel * resource_model = nullptr;
	
	const aiScene *scene = aiImportFile(asset_path, flags);
	
	if (scene != nullptr)
	{
		resource_model = App->resource_manager->CreateResource<ResourceModel>(model_uid);
		resource_model->asset_source = asset_path;

		if (scene->HasMaterials())
		{
			resource_model->textures_uid.reserve(scene->mNumMaterials);

			for (uint i = 0u; i < scene->mNumMaterials; ++i)
			{
				aiMaterial * material = scene->mMaterials[i];
				ResourceTexture * resource_texture = ImportFBXTexture(material, prev_textures_uids, asset_path);
				//TODO: Remove this if when we separate ResourceMaterials from ResourceTextures
				if (resource_texture == nullptr)
				{
					resource_model->textures_uid.push_back(INVALID_RESOURCE_UID);
				}
				else
				{
					resource_model->textures_uid.push_back(resource_texture->GetUID());
				}
			}
		}
		std::vector<uint> mesh_texture_indices;
		if (scene->HasMeshes())
		{
			resource_model->meshes_uid.reserve(scene->mNumMeshes);
			mesh_texture_indices.reserve(scene->mNumMeshes);

			for (uint i = 0u; i < scene->mNumMeshes; ++i)
			{
				aiMesh *assimp_mesh = scene->mMeshes[i];
				ResourceMesh * resource_mesh = App->import_mesh->ImportAssimpMesh(assimp_mesh, PopFirst(prev_meshes_uids), asset_path);
				resource_model->meshes_uid.push_back(resource_mesh->GetUID());
				mesh_texture_indices.push_back(resource_model->textures_uid[assimp_mesh->mMaterialIndex]);
			}
		}
		ImportFBXNodes(resource_model, new ModelNode(), scene->mRootNode, mesh_texture_indices, INVALID_MODEL_ARRAY_INDEX);
		if (scene->HasAnimations())
		{
			resource_model->animations_uid.reserve(scene->mNumAnimations);
			for (uint i = 0u; i < scene->mNumAnimations; ++i)
			{
				ResourceAnimation * resource_animation = nullptr;
				resource_animation = App->resource_manager->CreateResource<ResourceAnimation>(PopFirst(animation_uids));
				resource_animation->ImportAnimation((*scene->mAnimations[i]));
				resource_animation->asset_source = asset_path;
				resource_model->animations_uid.push_back(resource_animation->GetUID());
			}
		}

		aiReleaseImport(scene);
		resource_model->SaveFileData();
		SaveModelMeta(resource_model, asset_path);

		LOG("Success importing model from: %s in %i ms.", asset_path, import_timer.Read());
	}
	else
	{
		LOG("Error importing model from: %s.", asset_path);
	}

	import_timer.ReadSec();

	return resource_model;
}

void ModuleImportModel::SaveModelMeta(ResourceModel * resource_model, const char * asset_path)
{
	JSONFile meta_file;
	meta_file.CreateJSONFile();
	App->resource_manager->SaveUID(&meta_file, resource_model->uid);
	resource_model->SaveModifiedDate(&meta_file, asset_path);
	App->resource_manager->SaveUIDArray(resource_model->textures_uid, "exportedTextures", &meta_file);
	App->resource_manager->SaveUIDArray(resource_model->meshes_uid, "exportedMeshes", &meta_file);
	App->resource_manager->SaveUIDArray(resource_model->animations_uid, "exportedAnimations", &meta_file);
	//TODO: Add import options
	meta_file.SaveFile(std::string(asset_path) + std::string(".") + std::string(META_EXTENSION));
	meta_file.CloseFile();
}

void ModuleImportModel::LoadModelMeta(ResourceModel * model, const char * meta_path)
{
	JSONFile meta_file;
	meta_file.LoadFile(meta_path);

	//INFO: Load UID

	model->uid = App->resource_manager->LoadUID(&meta_file);

	//TODO: Create objects forcing their uids
	//App->resource_manager->CreateResource<ResourceModel>(uid);
	//meta_file.CloseFile();
}

bool ModuleImportModel::ImportFBXNodes(ResourceModel * resource_model, ModelNode * model_node, aiNode * node, const std::vector<uint> mesh_texture_idxs, uint parent_index)
{
	uint curr_index = resource_model->nodes.size();

	const char * node_name = node->mName.C_Str();
	model_node->name = new char[NODE_NAME_SIZE];
	memset(model_node->name, NULL, NODE_NAME_SIZE);
	strcpy(model_node->name, node_name);

	model_node->transform = reinterpret_cast<const float4x4&>(node->mTransformation);

	model_node->parent_index = parent_index;

	if (resource_model->meshes_uid.size() > 0u)
	{
		for (uint i = 0u; i < node->mNumMeshes; ++i)
		{
			uint mesh_index = node->mMeshes[i];
			model_node->mesh_uid = resource_model->meshes_uid[mesh_index];
			model_node->material_uid = mesh_texture_idxs[mesh_index];
		}
		//TODO: Create a new ResourceModelNode for each mesh
		//Right now this for allows that there is more than one mesh per gameobject
	}
	resource_model->nodes.push_back(model_node);

	for (uint i = 0u; i < node->mNumChildren; ++i)
	{
		ImportFBXNodes(resource_model, new ModelNode(), node->mChildren[i], mesh_texture_idxs, curr_index);
	}

	return true;
}

//INFO: textures must be in the same folder as the fbx
ResourceTexture * ModuleImportModel::ImportFBXTexture(const aiMaterial * material, std::vector<UID> & uids, const char * asset_path)
{
	ResourceTexture * ret = nullptr;

	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString aipath;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aipath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string name_file;
			App->file_system->SplitFilePath(aipath.C_Str(), nullptr, &name_file, nullptr);
			std::string file_path;
			App->file_system->SplitFilePath(asset_path, &file_path, nullptr, nullptr);
			std::string final_path = file_path + name_file;
			//INFO: Because textures are stored as individual files outside the FBX we might have imported the texture earlier
			std::string meta_path = (final_path + "." + META_EXTENSION);
			if (App->file_system->FileExists(meta_path.c_str()))
			{
				JSONFile file;
				file.LoadFile(meta_path);
				UID uid = file.LoadUID("resourceUID");
				ret = (ResourceTexture*)App->resource_manager->GetResource(uid);
			}
			else
			{
				ret = App->import_texture->ImportTexture(final_path.c_str(), PopFirst(uids));
				ret->asset_source = asset_path;
			}
		}
		else
		{
			LOG("Error: Texture path not found");
		}
	}
	else if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_AMBIENT) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_SHININESS) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_OPACITY) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_LIGHTMAP) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_REFLECTION) > 0)
	{
		LOG("texture type");
	}
	else if (material->GetTextureCount(aiTextureType_UNKNOWN) > 0)
	{
		LOG("texture type");
	}
	return ret;
}

void ModuleImportModel::CreateGameObjectFromModel(ResourceModel * resource_model, ComponentTransform * parent)
{
	std::vector<GameObject*> model_gameobjects;

	//Loads the data
	resource_model->StartUsingResource();

	for (uint i = 0u; i < resource_model->nodes.size(); ++i)
	{
		GameObject * new_gameobject = new GameObject(resource_model->nodes[i]->name, nullptr);
		new_gameobject->transform->SetGlobalMatrix(resource_model->nodes[i]->transform);
		
		if (resource_model->nodes[i]->mesh_uid != INVALID_RESOURCE_UID)
		{
			ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_manager->GetResource(resource_model->nodes[i]->mesh_uid);
			component_mesh->SetMesh(resource_mesh);
			new_gameobject->transform->bounding_box.SetLocalAABB(resource_mesh->aabb);
			new_gameobject->transform->bounding_box.MultiplyByMatrix(new_gameobject->transform->GetGlobalMatrix());
			//INFO: component mesh creates a material component inside its constructor
			ComponentMaterial * component_material = new_gameobject->GetComponent<ComponentMaterial>();
			component_material->SetTexture((ResourceTexture*)App->resource_manager->GetResource(resource_model->nodes[i]->material_uid));
		}
		model_gameobjects.push_back(new_gameobject);
	}

	//Parent them
	for (uint i = 0u; i < model_gameobjects.size(); ++i)
	{
		if (resource_model->nodes[i]->parent_index != INVALID_MODEL_ARRAY_INDEX)
		{
			model_gameobjects[i]->transform->SetParent(model_gameobjects[resource_model->nodes[i]->parent_index]->transform);
		}
	}

	if (resource_model->animations_uid.size() > 0)
	{
		ComponentAnimator * animator = model_gameobjects[0]->CreateComponent<ComponentAnimator>();

		//TODO Delete this part- We are not going to add clips directly to component Animator;
		for (auto iter = resource_model->animations_uid.begin(); iter != resource_model->animations_uid.end(); ++iter)
		{
			ResourceAnimation * resource_animation = (ResourceAnimation*)App->resource_manager->GetResource((*iter));
			animator->AddClip(resource_animation);
		}
	}
	resource_model->animations_uid.clear();
	//Set the root of the model to the scene
	model_gameobjects[0]->transform->SetParent(parent);

	resource_model->StopUsingResource();
}

bool ModuleImportModel::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

void ModuleImportModel::EventRequest(const Event &event)
{
	if (event.type == Event::EVENT_TYPE::DROPPED_FILE)
	{
		std::string extension;
		App->file_system->NormalizePath((char*)event.path);
		App->file_system->GetExtension(event.path, extension);

		std::string file;
		App->file_system->SplitFilePath(event.path, nullptr, &file, nullptr);
		//TODO: Change it so that you can drag onto an specific folder on the assets
		std::string dst_path = std::string(ASSETS_FOLDER) + std::string("/") + file;
		uint resource_type = App->resource_manager->GetResourceTypeFromExtension(extension);
		if (resource_type == ResourceTexture::type)
		{
			if (App->file_system->CopyFromOutsideFS(event.path, dst_path.c_str()))
			{
				App->import_texture->ImportTexture(dst_path.c_str());
				//TODO: Update assets tree
			}
		}
		else if (resource_type == ResourceModel::type)
		{
			if (App->file_system->CopyFromOutsideFS(event.path, dst_path.c_str()))
			{
				ImportModel(dst_path.c_str());
				//TODO: Update assets tree
			}
		}
		else
		{
			LOG("This type of file is not supported.");
		}
		LOG("File dropped %s", event.path);
	}
}

UID ModuleImportModel::PopFirst(std::vector<UID> & vector)
{
	if (vector.size() > 0u)
	{
		UID uid = (*vector.begin());
		vector.erase(vector.begin());
		return uid;
	}
	return INVALID_RESOURCE_UID;
}