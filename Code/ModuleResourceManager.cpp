#include "ModuleResourceManager.h"

ModuleResourceManager::ModuleResourceManager(const char * name) : Module(true, name)
{
}

Resource * ModuleResourceManager::GetResource(UID uid)
{
	Resource * resource = nullptr;
	auto iter = resources.find(uid);
	if (iter != resources.end())
	{
		resource = iter->second;
	}
	else
	{
		LOG("Error: The uid specified is not in the resources map.");
	}
	return resource;
}

UID ModuleResourceManager::GenerateNewUID()
{
	++last_uid;
	return last_uid;
}
