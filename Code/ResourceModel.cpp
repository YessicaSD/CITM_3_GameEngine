#include "ResourceModel.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResourceManager.h"

RESOURCE_DEFINITION(Resource, ResourceModel);

ResourceModel::~ResourceModel()
{
	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		RELEASE_ARRAY((*iter)->name);
	}
}

const char * ResourceModel::GetTypeString()
{
	return "model";
}

bool ResourceModel::SaveFileData()
{
	uint header_bytes = sizeof(uint) + sizeof(uint);
	uint node_name_bytes = sizeof(char) * NODE_NAME_SIZE;
	uint node_transform_bytes = sizeof(float) * 16u;
	uint node_parent_index_bytes = sizeof(uint);
	uint node_mesh_uid_bytes = sizeof(UID);
	uint node_material_uid_bytes = sizeof(UID);
	uint animation_size = sizeof(UID);

	uint node_size = node_name_bytes
		+ node_transform_bytes
		+ node_parent_index_bytes
		+ node_mesh_uid_bytes
		+ node_material_uid_bytes;

	uint total_size = header_bytes
		+ node_size * nodes.size()
		+ animation_size * animations_uid.size();

	char * data = new char[total_size];
	char * cursor = data;

	uint header[] = {
		nodes.size(),
		animations_uid.size()
	};
	SaveVariable(header, &cursor, header_bytes);

	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		SaveVariable((*iter)->name, &cursor, node_name_bytes);
		SaveVariable(&(*iter)->transform, &cursor, node_transform_bytes);
		SaveVariable(&(*iter)->parent_index, &cursor, node_parent_index_bytes);
		SaveVariable(&(*iter)->mesh_uid, &cursor, node_mesh_uid_bytes);
		SaveVariable(&(*iter)->material_uid, &cursor, node_material_uid_bytes);
	}

	//We'll end up with dupliate data (in the custom format and in the .meta) because we need the animation uids for both
	//- Reconstructing the scene when the resources folder isn't there.
	//- 
	for (auto iter = animations_uid.begin(); iter != animations_uid.end(); ++iter)
	{
		SaveVariable(&(*iter), &cursor, animation_size);
	}

	uint path_size = 250u;
	char * path = new char[path_size];
	App->file_system->CreatePath(path, path_size, RESOURCES_MODEL_FOLDER, uid, MODEL_EXTENSION);
	App->file_system->SaveFile(data, total_size, &path);

	return false;
}

bool ResourceModel::LoadFileData()
{
	Timer load_timer;

	char * data = nullptr;
	uint path_size = 250u;
	char * path = new char[path_size];
	App->file_system->CreatePath(path, path_size, RESOURCES_MODEL_FOLDER, uid, MODEL_EXTENSION);
	App->file_system->LoadFile(path, &data);
	char * cursor = data;

	//Load header
	//TODO: If it's only one variable that it's on the header, make a single variable instead of an array
	//Called num_nodes (it's more descriptive)
	//INFO: The number of elements on the ranges array must be the same as in the ranges array of SaveFileData()
	uint header[2];
	LoadVariable(header, &cursor, sizeof(header));
	uint num_nodes = header[0];
	uint num_animations = header[1];
	nodes.reserve(num_nodes);

	uint name_bytes = NODE_NAME_SIZE * sizeof(char);

	//Load nodes
	for (uint i = 0u; i < num_nodes; ++i)
	{
		ModelNode * node = new ModelNode();
		node->name = new char[NODE_NAME_SIZE];
		//INFO: Clear the name
		//TODO: See if it's necessary if we're copy info from a file with the same name length and which has \0 character at the end
		//memset(nodes[i]->name, 0, name_bytes);
		LoadVariable(node->name, &cursor, name_bytes);
		for (uint rows = 0u; rows < 4u; ++rows)
		{
			LoadVariable(&node->transform[rows], &cursor, sizeof(float) * 4u);
		}
		LoadVariable(&node->parent_index, &cursor, sizeof(uint));
		LoadVariable(&node->mesh_uid, &cursor, sizeof(UID));
		LoadVariable(&node->material_uid, &cursor, sizeof(UID));

		nodes.push_back(node);
	}

	for (int i = 0; i < num_animations; ++i)
	{
		UID animation_uid = 0;
		LoadVariable(&animation_uid, &cursor, sizeof(UID));
		animations_uid.push_back(animation_uid);
	}

	LOG("Success loading model nodes from: %s in: %i ms.", path, load_timer.Read());

	return true;
}

bool ResourceModel::ReleaseData()
{
	if (nodes.size() > 0u)
	{
		for (int i = nodes.size() - 1; i >= 0; --i)
		{
			RELEASE_ARRAY(nodes[i]->name);
			nodes[i]->transform = float4x4::identity;
			nodes[i]->material_uid = INVALID_MODEL_ARRAY_INDEX;
			nodes[i]->mesh_uid = INVALID_MODEL_ARRAY_INDEX;
			nodes[i]->parent_index = INVALID_MODEL_ARRAY_INDEX;
			RELEASE(nodes[i]);
		}
		nodes.clear();
	}
	if (animations_uid.size() > 0u)
	{
		for (auto iter = animations_uid.begin(); iter != animations_uid.end(); ++iter)
		{
			Resource * resource_animation = App->resource_manager->GetResource((*iter));
			resource_animation->ReleaseData();
		}
		animations_uid.clear();
	}
	if (meshes_uid.size() > 0u)
	{
		for (auto iter = meshes_uid.begin(); iter != meshes_uid.end(); ++iter)
		{
			Resource * resource_mesh = App->resource_manager->GetResource((*iter));
			resource_mesh->ReleaseData();
		}
		meshes_uid.clear();
	}
	if (textures_uid.size() > 0u)
	{
		for (auto iter = textures_uid.begin(); iter != textures_uid.end(); ++iter)
		{
			//TODO: Remove when we start using resource Material (there shouldn't be any invalid at that point)
			if ((*iter) != INVALID_RESOURCE_UID)
			{
				Resource * resource_texture = App->resource_manager->GetResource((*iter));
				resource_texture->ReleaseData();
			}
		}
		textures_uid.clear();
	}
	return true;
}


//Used in resource manager. Tells it that a particular uid exits by creating an instance on the map.
//void CreateResourcesInMap()
//{
//
//}
