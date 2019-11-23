#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include <vector>
#include <map>

#include "Module.h"
#include "Globals.h"
#include "Resource.h"

class ResourceTexture;

//Used to display the assets in the PanelAssets
struct AssetFile
{
	//type (used to get the appropiate icon)
	std::string name;
};

struct Dir {
	std::string name;
	std::vector<AssetFile*>assets;
	std::vector<Dir*>dirs;
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char * name);

	bool Start(JSONFile * module_file) override;

	template<class ResourceType>
	ResourceType * CreateResource(UID uid = 0u)
	{
		ResourceType * resource = new ResourceType();
		if (uid == INVALID_RESOURCE_UID)
		{
			resource->uid = GenerateNewUID();
		}
		else
		{
			resource->uid = uid;
		}
		resources[resource->uid] = resource;
		return resource;
	}

	update_status PreUpdate() override;
	bool CleanUp() override;

	Resource * GetResource(UID uid);

private:
	UID GenerateNewUID();

	void FillAssetTreeRecursive(Dir * dir);
	void DeleteTreeRecursive(Dir * dir);

private:
	std::map<UID, Resource*> resources;

	//Used to diplay the assets in PanelAssets
	Dir * asset_dir = nullptr;

	friend class PanelAssets;
	friend class PanelResources;
};


#endif