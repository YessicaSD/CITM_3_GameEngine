#ifndef __RESOURCE_BONE_H__
#define __RESOURCE_BONE_H__

#include "Resource.h"
#include "MathGeoLib/include/Math/float4x4.h"
struct VertexWeigth
{
	uint vertex_id = 0u;
	float weigth = 0.f;
};
struct aiBone;
class ResourceBone : public Resource
{
	RESOURCE_DECLARATION(ResourceBone);

private:
	char * name = nullptr;
	uint num_weights = 0u;
	VertexWeigth * weights = nullptr;
	float4x4 offset_matrix = float4x4::identity;

private:
	ResourceBone();//Constructor is private because Resources need to be created from ModuleResourceManager
	
	const char * GetTypeString() override;
	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

	friend class ResourceMesh;
	friend class ModuleImportBone;
	friend class ModuleResourceManager;
};

#endif