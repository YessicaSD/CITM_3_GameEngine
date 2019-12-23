//Our own custom format to load 3D models
//One ResourceModel is generated for every FBX

//It holds the following information:
//- Node structure
//- References to the meshes (can have more than one of them)
//- References to the materials (can have more than one of them)

#ifndef __RESOURCE_MODEL_H__
#define __RESOURCE_MODEL_H__

#define NODE_NAME_SIZE 250

#include <string>
#include <vector>

#include "Resource.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Globals.h"

#include "limits.h"
#define INVALID_MODEL_ARRAY_INDEX UINT_MAX
//If there is a model that has more meshes or textures than uint max it will give errors
//We can't use 0 because it points to the first element
//Best solution I can think of is to have another variable a bool or an int with different individual bytes
//Where we specify if it has a mesh, a material or a parent

struct ModelNode {
	char * name = nullptr;
	float4x4 transform = float4x4::identity;
	UID mesh_uid = INVALID_RESOURCE_UID;
	UID material_uid = INVALID_RESOURCE_UID;
	uint parent_index = INVALID_MODEL_ARRAY_INDEX;
};
//TODO: Alert when a Node has more than one mesh or material

class ResourceModel : public Resource
{
	RESOURCE_DECLARATION(ResourceModel);

public:
	~ResourceModel();

public:
	const char * GetTypeString() override;

private:
	ResourceModel() {};
	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

public:
	std::vector<ModelNode*> nodes;
	std::vector<UID> meshes_uid;
	std::vector<UID> textures_uid;
	std::vector<UID> animations_uid;

	friend class ModuleImportModel;
	friend class ModuleResourceManager;
};


#endif
