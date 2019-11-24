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
	std::string full_path;
};

struct AssetDir {
	std::string name;
	std::string full_path;

	std::vector<AssetFile*>assets;
	std::vector<AssetDir*>dirs;
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char * name);

	bool Start(JSONFile * module_file) override;

	template<class ResourceType>
	ResourceType * CreateResource(UID uid = INVALID_RESOURCE_UID)
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
	const char * GetResourceTypeString(uint type);

	UID LoadUID(JSONFile * meta_file) const;
	void SaveUID(JSONFile * meta_file, const UID & uid) const;
	void SaveUIDArray(const std::vector<UID>& uid_vector, char * name, JSONFile * meta_file) const;

private:
	
	//Checking assets helper functions
	void ImportAssetsRecursively(AssetDir * dir);
	void FillAssetTreeRecursive(AssetDir * dir);
	void DeleteTreeRecursive(AssetDir * dir);

	//Updating assets helper functions
	void ImportResource(const uint type, const char * path);
	void CreateResourcesInMap(const uint &type, JSONFile &meta_file, AssetFile * asset_file);
	void ReImportResources(JSONFile &meta_file, const uint &type, AssetFile * asset_file);
	
	//Helpers to the functions to update assets
	void DeleteDependantResources(std::vector<UID> uids, const char * name, JSONFile * meta_file, const char * folder, const char * extension);
	bool IsFileModified(JSONFile &meta_file, const char * file);
	bool MissingResources(JSONFile & meta_file, uint type);

	UID GenerateNewUID();

private:
	std::map<UID, Resource*> resources;

	//Used to diplay the assets in PanelAssets
	AssetDir * asset_dir = nullptr;

	friend class PanelAssets;
	friend class PanelResources;
};


#endif