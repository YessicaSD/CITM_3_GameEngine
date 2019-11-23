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

struct AssetDir {
	std::string name;
	std::vector<AssetFile*>assets;
	std::vector<AssetDir*>dirs;
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char * name);

	bool Start(JSONFile * module_file) override;

	void ImportAssetsRecursively(AssetDir * dir, std::string curr_dir);

	bool IsFileModified(JSONFile &meta_file, const char * file);

	bool MissingResources(JSONFile & meta_file, uint type);

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
	uint GetResourceTypeFromExtension(const std::string & extension);

	UID LoadUID(JSONFile * meta_file) const;
	void SaveUID(JSONFile * meta_file, const UID & uid) const;
	void SaveUIDArray(const std::vector<UID>& uid_vector, char * name, JSONFile * meta_file) const;

private:
	UID GenerateNewUID();

	void FillAssetTreeRecursive(AssetDir * dir);
	void DeleteTreeRecursive(AssetDir * dir);

private:
	std::map<UID, Resource*> resources;

	//Used to diplay the assets in PanelAssets
	AssetDir * asset_dir = nullptr;

	friend class PanelAssets;
	friend class PanelResources;
};


#endif