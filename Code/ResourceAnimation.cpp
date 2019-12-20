#include "ResourceAnimation.h"
#include "Assimp/include/scene.h"
#include "Globals.h"
#include "ModuleImport.h"
#include "ResourceModel.h"
#include "ModuleFileSystem.h"
#include "Application.h"

RESOURCE_DEFINITION(Resource, ResourceAnimation);

ResourceAnimation::~ResourceAnimation()
{
	CleanUp();
}

const char * ResourceAnimation::GetTypeString()
{
	return "animation";
}

void ResourceAnimation::CleanUp()
{
	RELEASE(nodes);
	name = "";
	num_channels = 0;
	duration = 0;
	ticks_per_second = 0;
}

bool ResourceAnimation::SaveFileData()
{
	bool ret = true;

	//Calculate the size of the data array
	uint size = 
		NODE_NAME_SIZE//name
		+ sizeof(duration)
		+ sizeof(ticks_per_second)
		+ sizeof(num_channels);

	for (int i = 0; i < num_channels; ++i)
	{
		size += NODE_NAME_SIZE;//name

		size += sizeof(nodes[i].num_position_keys);
		size += sizeof(nodes[i].num_rotation_keys);
		size += sizeof(nodes[i].num_scale_keys);

		size += sizeof(KeyAnimation<float3>) * nodes[i].num_position_keys;//TODO: Try if we can do sizeof(position_keys)
		size += sizeof(KeyAnimation<Quat>) * nodes[i].num_rotation_keys;
		size += sizeof(KeyAnimation<float3>) * nodes[i].num_scale_keys;
	}

	//Save the data array
	char* data = new char[size];
	char* cursor = data;
	SaveVariable(&name, &cursor,sizeof(char) * NODE_NAME_SIZE);
	SaveVariable(&duration, &cursor,sizeof(duration));
	SaveVariable(&ticks_per_second, &cursor,sizeof(ticks_per_second));
	SaveVariable(&num_channels, &cursor,sizeof(num_channels));

	for (int i = 0; i < num_channels; ++i)
	{
		uint ranges[] = {
			nodes[i].num_position_keys,
			nodes[i].num_rotation_keys,
			nodes[i].num_scale_keys
		};

		SaveVariable(&name, &cursor, NODE_NAME_SIZE);
		SaveVariable(ranges, &cursor, sizeof(ranges));
		SaveVariable(nodes[i].position_keys, &cursor, sizeof(KeyAnimation<float3>) * nodes[i].num_position_keys);
		SaveVariable(nodes[i].rotation_keys, &cursor, sizeof(KeyAnimation<Quat>) * nodes[i].num_rotation_keys);
		SaveVariable(nodes[i].scale_keys, &cursor, sizeof(KeyAnimation<float3>) * nodes[i].num_scale_keys);
	}

	//Save file
	uint path_size = 250u;
	char * path = new char[path_size];
	App->file_system->CreatePath(path, path_size, RESOURCES_ANIMATION_FOLDER, uid, ANIMATION_EXTENSION);
	ret = App->file_system->SaveFile((const void *)data, size, &path);
	RELEASE_ARRAY(path);

	return ret;
}

bool ResourceAnimation::LoadFileData()
{
	Timer load_timer;

	//Create path
	char * data = nullptr;
	uint path_size = 250u;
	char * path = new char[path_size];
	App->file_system->CreatePath(path, path_size, RESOURCES_ANIMATION_FOLDER, uid, ANIMATION_EXTENSION);

	bool ret = App->file_system->LoadFile(path, &data);
	if (ret)
	{
		char * cursor = data;

		LoadVariable(&name, &cursor, NODE_NAME_SIZE);
		LoadVariable(&duration, &cursor, sizeof(duration));
		LoadVariable(&ticks_per_second, &cursor, sizeof(ticks_per_second));
		LoadVariable(&num_channels, &cursor, sizeof(num_channels));

		for (int i = 0; i < num_channels; ++i)
		{
			LoadVariable(name, &cursor, NODE_NAME_SIZE);
			
			//INFO: The number of elements on the ranges array must be the same as in the ranges array of the ResourceAnimation::SaveFileData()
			uint ranges[3];
			LoadVariable(ranges, &cursor, sizeof(ranges));
			nodes[i].num_position_keys = ranges[0];
			nodes[i].num_rotation_keys = ranges[1];
			nodes[i].num_scale_keys = ranges[2];

			LoadVariable(nodes[i].position_keys, &cursor, sizeof(KeyAnimation<float3>) * nodes[i].num_position_keys);
			LoadVariable(nodes[i].rotation_keys, &cursor, sizeof(KeyAnimation<Quat>) * nodes[i].num_rotation_keys);
			LoadVariable(nodes[i].scale_keys, &cursor, sizeof(KeyAnimation<float3>) * nodes[i].num_scale_keys);

			LOG("Success loading animation from: %s in : %i ms.", path, load_timer.Read());
		}
	}

	RELEASE_ARRAY(path);

	return ret;
}

bool ResourceAnimation::ReleaseData()
{
	if (num_channels > 0u)
	{
		for (int i = num_channels - 1; i < num_channels; ++i)
		{
			RELEASE_ARRAY(nodes[i].scale_keys);
			RELEASE_ARRAY(nodes[i].rotation_keys);
			RELEASE_ARRAY(nodes[i].position_keys);
			nodes[i].num_scale_keys = 0u;
			nodes[i].num_rotation_keys = 0u;
			nodes[i].num_position_keys = 0u;
			RELEASE_ARRAY(name);
		}
		RELEASE_ARRAY(nodes);
	}

	num_channels = 0u;
	ticks_per_second = 0.f;
	duration = 0.f;
	RELEASE_ARRAY(name);

	return true;
}

void ResourceAnimation::ImportAnimation(const aiAnimation& animation)
{
	const char * animation_name = animation.mName.C_Str();
	name = new char[NODE_NAME_SIZE];
	memset(name, NULL, NODE_NAME_SIZE);
	strcpy(name, animation_name);

	duration = animation.mDuration;
	ticks_per_second = animation.mTicksPerSecond;
	num_channels = animation.mNumChannels;

	nodes = new NodeAnimation[num_channels];
	for (uint i = 0u; i < num_channels; ++i)
	{
		nodes[i].ImportAnimationNode((*animation.mChannels[i]));
	}

	SaveFileData();
}

void NodeAnimation::ImportAnimationNode(const aiNodeAnim & node_animation)
{
	const char * node_name = node_animation.mNodeName.C_Str();
	name = new char[NODE_NAME_SIZE];
	memset(name, NULL, NODE_NAME_SIZE);
	strcpy(name, node_name);//TODO: Make a function for this
	
	num_position_keys = node_animation.mNumPositionKeys;
	num_rotation_keys = node_animation.mNumRotationKeys;
	num_scale_keys = node_animation.mNumScalingKeys;

	position_keys = new KeyAnimation<float3>[num_position_keys]();
	rotation_keys = new KeyAnimation<Quat>[num_rotation_keys]();
	scale_keys = new KeyAnimation<float3>[num_scale_keys]();

	aiVectorKey* m_position_keys = node_animation.mPositionKeys;
	for (int i = 0; i < num_position_keys; ++i)
	{
		position_keys[i].time = m_position_keys[i].mTime;
		position_keys[i].value.Set(m_position_keys[i].mValue.x, m_position_keys[i].mValue.y, m_position_keys[i].mValue.z) ;
	}

	aiQuatKey* m_rotation_keys = node_animation.mRotationKeys;
	for (int i = 0; i < num_rotation_keys; ++i)
	{
		rotation_keys[i].time = m_rotation_keys[i].mTime;
		rotation_keys[i].value.Set(m_rotation_keys[i].mValue.x, m_rotation_keys[i].mValue.y, m_rotation_keys[i].mValue.z, m_rotation_keys[i].mValue.w);
	}

	aiVectorKey* m_scale_keys = node_animation.mScalingKeys;
	for (int i = 0; i < num_scale_keys; ++i)
	{
		scale_keys[i].time = m_scale_keys[i].mTime;
		scale_keys[i].value.Set(m_scale_keys[i].mValue.x, m_scale_keys[i].mValue.y, m_scale_keys[i].mValue.z);
	}
}
