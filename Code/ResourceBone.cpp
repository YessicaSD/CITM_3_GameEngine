#include "ResourceBone.h"
#include "Assimp/include/mesh.h"
#include "Application.h"
#include "ModuleResourceManager.h"

RESOURCE_DEFINITION(Resource, ResourceBone);

ResourceBone::ResourceBone()
{
}

const char *ResourceBone::GetTypeString()
{
	return "bone";
}

bool ResourceBone::SaveFileData()
{
	return true;
}

bool ResourceBone::LoadFileData()
{

	return true;
}

bool ResourceBone::ReleaseData()
{
	return true;
}
