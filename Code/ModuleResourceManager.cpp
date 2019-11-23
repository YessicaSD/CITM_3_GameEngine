#include "ModuleResourceManager.h"
#include "ModuleRandom.h"
#include "Application.h"

ModuleResourceManager::ModuleResourceManager(const char * name) : Module(true, name)
{
}

//TODO:
//Start()
//Check for new assets

//ModuleGUI
//+ Refresh button to check for new resources

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
	return App->random->RandomUID();
}
