#include "ResourceBone.h"
#include "Assimp/include/mesh.h"

RESOURCE_DEFINITION(Resource, ResourceBone);

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

void ResourceBone::LoadFromAssimp(aiBone bone)
{
	name = bone.mName.C_Str();

}
