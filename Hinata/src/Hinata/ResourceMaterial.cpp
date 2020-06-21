#include "ResourceMaterial.h"

RESOURCE_DEFINITION(Resource, ResourceMaterial);

const char * ResourceMaterial::GetTypeString()
{
	return "material";
}

bool ResourceMaterial::SaveFileData()
{
	return true;
}

bool ResourceMaterial::LoadFileData()
{
	return true;
}

bool ResourceMaterial::ReleaseData()
{
	return true;
}
