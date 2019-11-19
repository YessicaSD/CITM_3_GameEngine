#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include <vector>
#include <map>

#include "Module.h"
#include "Globals.h"
#include "Resource.h"

struct Asset {
	ResourceTexture* image;
	//Childs in meta that can be expanded
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char * name);

	template<class ResourceType>
	ResourceType * CreateNewResource()
	{
		ResourceType * resource = new ResourceType();
		resource->uid = GenerateNewUID();
		//TODO: Remove, only for testing purposes
		//Ideally resources would be in the map already and we would just need to load their data or not depending on if there is an / some object/s refecencing them or not
		resources[resource->GetUID()] = resource;
		return resource;
	}

	Resource * GetResource(UID uid);

private:
	UID GenerateNewUID();

private:
	UID last_uid = 0u;
	std::map<UID, Resource*> resources;

	//Used for the "Assets" window
	//Tree structure?
	std::list<Asset> assets;

	friend class PanelAssets;
};


#endif