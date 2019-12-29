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
#include "ComponentAnimator.h"
#include "ComponentSkinnedMesh.h"

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

#include "ResourceBone.h"

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
ResourceModel * ModuleImportModel::ImportModel(
	const char *asset_path,
	UID model_uid,
	std::vector<UID> & prev_meshes_uids,
	std::vector<UID> & prev_textures_uids,
	std::vector<UID>& animation_uids,
	std::vector<UID>& bones_uids)
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
				ResourceTexture * resource_texture = ImportModelTexture(material, prev_textures_uids, asset_path);
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
		std::unordered_set<std::string> bones;
		std::vector<uint> mesh_texture_indices;
		if (scene->HasMeshes())
		{
			resource_model->meshes_uid.reserve(scene->mNumMeshes);
			mesh_texture_indices.reserve(scene->mNumMeshes);

			for (uint i = 0u; i < scene->mNumMeshes; ++i)
			{
				aiMesh *assimp_mesh = scene->mMeshes[i];
				ResourceMesh * resource_mesh = App->import_mesh->ImportAssimpMesh(assimp_mesh, App->resource_manager->PopFirst(prev_meshes_uids), bones_uids, asset_path);
				resource_model->meshes_uid.push_back(resource_mesh->GetUID());
				mesh_texture_indices.push_back(resource_model->textures_uid[assimp_mesh->mMaterialIndex]);

				//Insert the nodes
				if (resource_mesh->HasBones())
				{
					for (int j = 0u; j < resource_mesh->num_bones; ++j)
					{
						bones.insert(resource_mesh->bones[j]->GetName());
					}
				}
			}
		}
		ImportModelNodes(resource_model, scene->mRootNode, mesh_texture_indices, INVALID_MODEL_ARRAY_INDEX, float4x4::identity);

		//Check which one is the root bone
		int curr_min_gen = INT_MAX;
		int curr_node_idx = 0;
		for (int i = 0; i < resource_model->nodes.size(); ++i)
		{
			if (bones.find(resource_model->nodes[i]->name) != bones.end())
			{
				int curr_gen = GetGeneration(resource_model, i);
				if (curr_gen < curr_min_gen)
				{
					curr_min_gen = curr_gen;
					curr_node_idx = i;
				}
			}
		}
		//TODO: Allow for more than one skeleton root (if there are various skeletons in the scene)
		//Instead of int curr_node_idx make a std::vector
		//If it's == add it
		//If it's > clear the vector and add it
		resource_model->root_bones.push_back(resource_model->nodes[curr_node_idx]->parent_index);
		//TODO: Check if all children on the bones hierarchy are going to be bones? You could add a sword in it.

		if (scene->HasAnimations())
		{
			resource_model->animations_uid.reserve(scene->mNumAnimations);
			for (uint i = 0u; i < scene->mNumAnimations; ++i)
			{
				ResourceAnimation * resource_animation = nullptr;
				resource_animation = App->resource_manager->CreateResource<ResourceAnimation>(App->resource_manager->PopFirst(animation_uids));
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
	std::vector<UID> bones_uids;
	for (int i = 0; i < resource_model->meshes_uid.size(); ++i)
	{
		ResourceMesh * mesh = (ResourceMesh*)App->resource_manager->GetResource(resource_model->meshes_uid[i]);
		for (int j = 0; j < mesh->num_bones; ++j)
		{
			bones_uids.push_back(mesh->bones[j]->GetUID());
		}
	}
	App->resource_manager->SaveUIDArray(bones_uids, "exportedBones", &meta_file);
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

//How many steps has it had to go through to find the parent bone
int ModuleImportModel::GetGeneration(ResourceModel * resource_model, int node_idx)
{
	int gen = 0;
	while (resource_model->nodes[node_idx]->parent_index != INVALID_MODEL_ARRAY_INDEX)
	{
		node_idx = resource_model->nodes[node_idx]->parent_index;
		gen++;
	}
	return gen;
}

bool ModuleImportModel::ImportModelNodes(ResourceModel * resource_model, aiNode * node, const std::vector<uint> & mesh_texture_idxs, uint parent_index, float4x4 curr_transformation)
{
	uint curr_index = INVALID_MODEL_ARRAY_INDEX;

	if (std::string(node->mName.C_Str()).find("_$AssimpFbx$_") == std::string::npos)
	{
		curr_index = resource_model->nodes.size();

		//If the name isn't _$Assimp$_ create a new node
		ModelNode * model_node = new ModelNode();

		const char * node_name = node->mName.C_Str();
		model_node->name = new char[NODE_NAME_SIZE];
		memset(model_node->name, NULL, NODE_NAME_SIZE);
		strcpy(model_node->name, node_name);

		//curr_transformation carries the transformation of the dummy "_$AssimpFbx$_" nodes
		//it's float::4x4 identity if there isnt' any
		model_node->transform = curr_transformation * reinterpret_cast<const float4x4&>(node->mTransformation);
		curr_transformation = float4x4::identity;

		model_node->parent_index = parent_index;

		if (resource_model->meshes_uid.size() > 0u)
		{
			for (uint i = 0u; i < node->mNumMeshes; ++i)
			{
				uint mesh_index = node->mMeshes[i];
				model_node->mesh_uid = resource_model->meshes_uid[mesh_index];
				model_node->material_uid = mesh_texture_idxs[mesh_index];
			}
			//INFO: We allow a single gameobject to have more than one mesh to preserve the same hierarchy as you'd see in the 3D program
		}
		resource_model->nodes.push_back(model_node);
	}
	else
	{
		curr_index = parent_index;
		//If the name is "_$Assimp$_" only grab the transformation and go to the child nodes
		curr_transformation = curr_transformation * reinterpret_cast<const float4x4&>(node->mTransformation);
	}

	for (uint i = 0u; i < node->mNumChildren; ++i)
	{
		ImportModelNodes(resource_model, node->mChildren[i], mesh_texture_idxs, curr_index, curr_transformation);
	}

	return true;
}

//INFO: textures must be in the same folder as the fbx
ResourceTexture * ModuleImportModel::ImportModelTexture(const aiMaterial * material, std::vector<UID> & uids, const char * asset_path)
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
				ret = App->import_texture->ImportTexture(final_path.c_str(), App->resource_manager->PopFirst(uids));
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

GameObject * ModuleImportModel::CreateGameObjectFromModel(ResourceModel * resource_model, ComponentTransform * parent)
{
	std::vector<GameObject*> model_gameobjects;

	//Loads the data
	resource_model->StartUsingResource();
	std::vector<ComponentSkinnedMesh*> skinned_mesh_comp_vector;
	for (uint i = 0u; i < resource_model->nodes.size(); ++i)
	{
		GameObject * new_gameobject = new GameObject(resource_model->nodes[i]->name, INVALID_GAMEOBJECT_UID);
		new_gameobject->transform->SetGlobalMatrix(resource_model->nodes[i]->transform);
		
		if (resource_model->nodes[i]->mesh_uid != INVALID_RESOURCE_UID)
		{
			ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_manager->GetResource(resource_model->nodes[i]->mesh_uid);
			resource_mesh->StartUsingResource();
			if (resource_mesh->HasBones())
			{
				ComponentSkinnedMesh* component_skinned_mesh = new_gameobject->CreateComponent<ComponentSkinnedMesh>();
				component_skinned_mesh->SetMesh(resource_mesh);
				skinned_mesh_comp_vector.push_back(component_skinned_mesh);
			}
			else
			{
				ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
				component_mesh->SetMesh(resource_mesh);
				new_gameobject->transform->bounding_box.SetLocalAABB(resource_mesh->aabb);
				new_gameobject->transform->bounding_box.MultiplyByMatrix(new_gameobject->transform->GetGlobalMatrix());
				//INFO: component mesh creates a material component inside its constructor
				ComponentMaterial * component_material = new_gameobject->GetComponent<ComponentMaterial>();
				component_material->SetTexture((ResourceTexture*)App->resource_manager->GetResource(resource_model->nodes[i]->material_uid));
			}
			
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

	if (resource_model->animations_uid.size() > 0u)
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
	for (std::vector<ComponentSkinnedMesh*>::iterator iter = skinned_mesh_comp_vector.begin(); iter != skinned_mesh_comp_vector.end(); ++iter)
	{
		(*iter)->root_object = model_gameobjects[0]->transform;
	}
	//Set the root of the model to the scene
	model_gameobjects[0]->transform->SetParent(parent);

	//resource_model->StopUsingResource();
	//TODO: Remove the resource when the animator is destroyed (it's the only one that makes use of it right now)
	return model_gameobjects[0];
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
		uint resource_type = App->resource_manager->GetAssetTypeFromExtension(extension);
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
				ImportModel(dst_path.c_str(), INVALID_RESOURCE_UID, std::vector<UID>(), std::vector<UID>(), std::vector<UID>(), std::vector<UID>());
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