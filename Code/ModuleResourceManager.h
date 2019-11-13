#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER__H__

#include <vector>
#include <map>

#include "Module.h"
#include "Globals.h"
#include "Resource.h"

#define INVALID_RESOURCE_UID 0

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char * name);

	template<class ResourceType>
	ResourceType * CreateNewResource()
	{
		ResourceType * resource = new ResourceType();
		resource->uid = GenerateNewUID();
		return resource;
	}

	Resource * GetResource(UID uid);

private:
	UID GenerateNewUID();

private:
	UID last_uid = 0u;
	std::map<UID, Resource*> resources;
};

#endif