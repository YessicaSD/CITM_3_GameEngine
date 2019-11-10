//Our own custom format to load 3D models
//One ResourceModel is generated for every FBX

//It holds the following information:
//- Node structure
//- References to the meshes (can have more than one of them)
//- References to the materials (can have more than one of them)

#ifndef __RESOURCE_MODEL_H__
#define __RESOURCE_MODEL_H__

#include<string>
#include<vector>
#include "MathGeoLib/include/Math/float4x4.h"
#include "Resource.h"
#include "Globals.h"

struct ResourceModelNode {
	std::string name;
	float4x4 transform = float4x4::identity;
	std::vector<UID> meshes;//A single node can have more than one mesh
	std::vector<UID> materials;//A single node can have more than one material
	uint parent_index = 0u;//The position of the parent on the node array
};

class ResourceModel : public Resource
{
public:

	std::vector<uint> mesh_indices;
	std::vector<ResourceModelNode*> nodes;
	//textures
	//translation? rotation? scale?
};

#endif
