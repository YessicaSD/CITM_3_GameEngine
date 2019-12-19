#include "ResourceAnimation.h"
#include "Assimp/include/scene.h"
#include "Globals.h"
#include "ModuleImport.h"
#include "ResourceModel.h"

RESOURCE_DEFINITION(Resource, ResourceAnimation);

ResourceAnimation::~ResourceAnimation()
{
	CleanUp();
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
	//Calculate the size of the data array
	uint size = 
		250u * sizeof(char)//name
		+ sizeof(duration)
		+ sizeof(ticks_per_second)
		+ sizeof(num_channels);

	for (int i = 0; i < num_channels; ++i)
	{
		size += 250u;//name

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
		uint ranges[3u];
		SaveVariable(ranges, &cursor, sizeof(uint) * 3u);
		//SaveVariable(nodes[i].position_keys, &cursor, sizeof(KeyAnimation<float3>));
		//SaveVariable(nodes[i].rotation_keys, &cursor, sizeof(KeyAnimation<float3>));
		//SaveVariable(nodes[i].scale_keys, &cursor, sizeof(KeyAnimation<float3>));
	}
	

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
	for (uint i = 0; i < num_channels; ++i)
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
	strcpy(name, node_name);
	
	num_position_keys = node_animation.mNumPositionKeys;
	num_scale_keys = node_animation.mNumScalingKeys;
	num_rotation_keys = node_animation.mNumRotationKeys;

	position_keys = new KeyAnimation<float3>[num_position_keys]();
	scale_keys = new KeyAnimation<float3>[num_scale_keys]();
	rotation_keys = new KeyAnimation<Quat>[num_rotation_keys]();

	aiVectorKey* m_position_keys = node_animation.mPositionKeys;
	for (int i = 0; i < num_position_keys; ++i)
	{
		position_keys[i].time = m_position_keys[i].mTime;
		position_keys[i].value.Set(m_position_keys[i].mValue.x, m_position_keys[i].mValue.y, m_position_keys[i].mValue.z) ;
	}
	aiVectorKey* m_scale_keys = node_animation.mScalingKeys;
	for (int i = 0; i < num_scale_keys; ++i)
	{
		scale_keys[i].time = m_scale_keys[i].mTime;
		scale_keys[i].value.Set(m_scale_keys[i].mValue.x, m_scale_keys[i].mValue.y, m_scale_keys[i].mValue.z);
	}
	aiQuatKey* m_rotation_keys = node_animation.mRotationKeys;
	for (int i = 0; i < num_rotation_keys; ++i)
	{
		rotation_keys[i].time = m_rotation_keys[i].mTime;
		rotation_keys[i].value.Set(m_rotation_keys[i].mValue.x, m_rotation_keys[i].mValue.y, m_rotation_keys[i].mValue.z, m_rotation_keys[i].mValue.w);
	}
}
