#include "ModuleResourceManager.h"

ModuleResourceManager::ModuleResourceManager(const char * name) : Module(true, name)
{
}

UID ModuleResourceManager::GenerateNewUID()
{
	++last_uid;
	return last_uid;
}
