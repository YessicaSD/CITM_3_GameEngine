#include "ResourceAnimation.h"
#include "Assimp/include/scene.h"
#include "Globals.h"
RESOURCE_DEFINITION(Resource, ResourceAnimation);

ResourceAnimation::~ResourceAnimation()
{
	CleanUp();
}

void ResourceAnimation::CleanUp()
{
	RELEASE(nodes);
	name = "";
	numChannels = 0;
	duration = 0;
	ticksPerSecond = 0;
}

bool ResourceAnimation::SaveFileData()
{
	uint ranges[] =
	{
		numChannels
	};

	return true;
}

void ResourceAnimation::LoadAnimation(const aiAnimation& animation)
{
	name = animation.mName.C_Str();
	duration = animation.mDuration;
	ticksPerSecond = animation.mTicksPerSecond;
	numChannels = animation.mNumChannels;
	nodes = new NodeAnimation[numChannels];
	for (uint i = 0; i < numChannels; ++i)
	{
		nodes[i].LoadNodeAnimation((*animation.mChannels[i]));
	}

}

void NodeAnimation::LoadNodeAnimation(const aiNodeAnim & node_animation)
{
	name = node_animation.mNodeName.C_Str();
	
	numPositionKeys = node_animation.mNumPositionKeys;
	numScalingKeys = node_animation.mNumScalingKeys;
	numRotationKeys = node_animation.mNumRotationKeys;

	position_keys = new KeyAnimation<float3>[numPositionKeys]();
	scale_keys = new KeyAnimation<float3>[numScalingKeys]();
	rotation_keys = new KeyAnimation<Quat>[numRotationKeys]();

	aiVectorKey* m_position_keys = node_animation.mPositionKeys;
	for (int i = 0; i < numPositionKeys; ++i)
	{
		position_keys[i].time = m_position_keys[i].mTime;
		position_keys[i].value.Set(m_position_keys[i].mValue.x, m_position_keys[i].mValue.y, m_position_keys[i].mValue.z) ;
	}
	aiVectorKey* m_scale_keys = node_animation.mScalingKeys;
	for (int i = 0; i < numScalingKeys; ++i)
	{
		scale_keys[i].time = m_scale_keys[i].mTime;
		scale_keys[i].value.Set(m_scale_keys[i].mValue.x, m_scale_keys[i].mValue.y, m_scale_keys[i].mValue.z);
	}
	aiQuatKey* m_rotation_keys = node_animation.mRotationKeys;
	for (int i = 0; i < numRotationKeys; ++i)
	{
		rotation_keys[i].time = m_rotation_keys[i].mTime;
		rotation_keys[i].value.Set(m_rotation_keys[i].mValue.x, m_rotation_keys[i].mValue.y, m_rotation_keys[i].mValue.z, m_rotation_keys[i].mValue.w);
	}
}
