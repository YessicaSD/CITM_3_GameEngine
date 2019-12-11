#ifndef RESOURCE_ANIMATION_H_
#define RESOURCE_ANIMATION_H_
#include "Resource.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include <string>

template<typename TYPE>
struct KeyAnimation
{
	double time;
	TYPE value;
};
class NodeAnimation
{
	std::string name;
	uint numPositionKeys;
	KeyAnimation<float3>* positionKeys = nullptr;
	uint numScalingKeys;
	KeyAnimation<float3>* positionKeys = nullptr;
	uint numRotationKeys;
	KeyAnimation<Quat>* positionKeys = nullptr;
};
class ResourceAnimation : public Resource
{
private:
	std::string name;
	float duration;
	float ticksPerSecond;
	uint numChannels;
	NodeAnimation* nodes = nullptr;

};
#endif // !RESOURCE_ANIMATION_H_

