#ifndef RESOURCE_ANIMATION_H_
#define RESOURCE_ANIMATION_H_
#include "Resource.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include <string>
#define DEFAULT_TICK_PER_SECOND 25

template<typename TYPE>
struct KeyAnimation
{
	double time;
	TYPE value;
};

struct aiNodeAnim;

class AnimationChannels
{
	char * name = nullptr;

	uint num_position_keys;
	KeyAnimation<float3>* position_keys = nullptr;

	uint num_rotation_keys;
	KeyAnimation<Quat>* rotation_keys = nullptr;

	uint num_scale_keys;
	KeyAnimation<float3>* scale_keys = nullptr;

	AnimationChannels* ImportAnimationNode(const aiNodeAnim& node_animation, float4x4& transformation);
	bool GetKeyFloat3(double time, math::float3 & value, KeyAnimation<float3>* keys, uint num_keys);
public:
	const char* GetName() {
		return name;
	}
	bool GetKeyPosition(double time, float3& value);
	bool GetKeyRotation(double time, Quat& value);
	bool GetKeyScale(double time, float3& value);

	friend class ResourceAnimation;
};

struct aiAnimation;

class ResourceAnimation : public Resource
{
	RESOURCE_DECLARATION(ResourceAnimation);
public:
	~ResourceAnimation();
	const char * GetTypeString() override;
	void CleanUp() override;
	uint GetNumChannels();
	AnimationChannels* GetChannels();
	float GetTicksPerSecond();
	double GetDuration();
protected:
	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

	void ImportAnimation(const aiAnimation& animation);

private:
	char * name = nullptr;
	double duration = 0.f;
	float ticks_per_second = 0.f;

	uint num_channels = 0u;
	AnimationChannels* channels = nullptr;
	
	friend class ModuleImportModel;
};
#endif // !RESOURCE_ANIMATION_H_

