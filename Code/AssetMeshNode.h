#ifndef __ASSET_MESH_NODE_H__
#define __ASSET_MESH_NODE_H__

#include <vector>
#include <string>

class AssetMesh;

class AssetMeshNode
{
	std::string name;
	std::vector<AssetMesh*> meshes;
	std::vector<AssetMeshNode*> children;
};

#endif
