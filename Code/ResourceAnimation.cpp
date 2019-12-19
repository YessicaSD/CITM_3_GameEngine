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
}
