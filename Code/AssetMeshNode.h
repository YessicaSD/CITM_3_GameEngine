#ifndef __ASSET_MESH_NODE_H__
#define __ASSET_MESH_NODE_H__

#include <vector>
#include <string>

class AssetMesh;

class AssetMeshNode
{
public:
	std::string name;
	std::vector<uint> mesh_indices;
	//textures
	//translation? rotation? scale?
};

#endif
