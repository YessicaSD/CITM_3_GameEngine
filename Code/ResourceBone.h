#ifndef __RESOURCE_BONE_H__
#define __RESOURCE_BONE_H__

#include "Resource.h"
#include "MathGeoLib/include/Math/float4x4.h"
struct VertexWeigth
{
	uint vertex_id = 0;
	float weigth = 0;
};
struct aiBone;
class ResourceBone : public Resource
{
	RESOURCE_DECLARATION(ResourceBone);

private:
	const char * name = nullptr;
	uint num_weights = 0;
	float4x4 offset_matrix = float4x4::identity;

private:
	ResourceBone();//Constructor is private because Resources need to be created from ModuleResourceManager

	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;
public:
	void LoadFromAssimp(aiBone bone);
};

#endif