//Our own custom format to load 3D models
//One ResourceModel is generated for every FBX

//It holds the following information:
//- Node structure
//- References to the meshes (can have more than one of them)
//- References to the materials (can have more than one of them)

#ifndef __RESOURCE_MODEL_H__
#define __RESOURCE_MODEL_H__

#define NODE_NAME_SIZE 250

#include<string>
#include<vector>
#include "MathGeoLib/include/Math/float4x4.h"
#include "Resource.h"
#include "Globals.h"

struct ResourceModelNode {
	char * name;
	float4x4 transform = float4x4::identity;
	UID mesh;
	UID material;
	uint parent_index = 0u;
};
//TODO: Alert when a Node has more than one mesh or material

class ResourceModel : public Resource
{
public:
	bool SaveFileData() override;
	bool LoadFileData(char * data) override;

public:
	std::vector<ResourceModelNode*> nodes;
	std::vector<UID> meshes_uid;
	std::vector<UID> textures_uid;
};


#endif
