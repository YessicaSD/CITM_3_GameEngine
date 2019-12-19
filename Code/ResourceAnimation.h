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
struct aiNodeAnim;
class NodeAnimation
{
	std::string name;
	uint numPositionKeys;
	KeyAnimation<float3>* position_keys = nullptr;
	uint numScalingKeys;
	KeyAnimation<float3>* scale_keys = nullptr;
	uint numRotationKeys;
	KeyAnimation<Quat>* rotation_keys = nullptr;

	void LoadNodeAnimation(const aiNodeAnim& node_animation);
	friend class ResourceAnimation;
};
struct aiAnimation;
class ResourceAnimation : public Resource
{
	RESOURCE_DECLARATION(ResourceAnimation);
public:
	~ResourceAnimation();

	 void CleanUp() override;
private:
	std::string name = "";
	float duration = 0;
	float ticksPerSecond = 0;
	uint numChannels = 0u;
	NodeAnimation* nodes = nullptr;

	void LoadAnimation(const aiAnimation& animation);
	friend class ModuleImport;
};
#endif // !RESOURCE_ANIMATION_H_

