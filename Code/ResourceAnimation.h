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

class AnimationChannels
{
	char * name = nullptr;

	uint num_position_keys;
	KeyAnimation<float3>* position_keys = nullptr;

	uint num_rotation_keys;
	KeyAnimation<Quat>* rotation_keys = nullptr;

	uint num_scale_keys;
	KeyAnimation<float3>* scale_keys = nullptr;

	void ImportAnimationNode(const aiNodeAnim& node_animation);
	
public:
	const char* GetName() {
		return name;
	}
	KeyAnimation<float3>* getKeyPosition(double time);
	KeyAnimation<float3>* getKeyScale(double time);
	KeyAnimation<Quat>* getKeyRotation(double time);

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

protected:
	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

	void ImportAnimation(const aiAnimation& animation);

private:
	char * name = nullptr;
	float duration = 0.f;
	float ticks_per_second = 0.f;

	uint num_channels = 0u;
	AnimationChannels* channels = nullptr;
	
	friend class ModuleImportModel;
};
#endif // !RESOURCE_ANIMATION_H_
