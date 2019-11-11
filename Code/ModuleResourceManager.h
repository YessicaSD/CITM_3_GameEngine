#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER__H__

#include <vector>

#include "Module.h"
#include "Globals.h"

class Resource;

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char * name);

	template<class ResourceType>
	ResourceType * CreateNewResource()
	{
		ResourceType * resource = new ResourceType();
		resource.uid = GenerateNewUID();
		return resource;
	}

private:
	UID GenerateNewUID();

private:
	UID last_uid = 0u;
	std::vector<Resource*> resources;
};

#endif