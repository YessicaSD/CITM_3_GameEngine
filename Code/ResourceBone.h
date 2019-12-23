#ifndef __RESOURCE_BONE_H__
#define __RESOURCE_BONE_H__

#include "Resource.h"

class ResourceBone : public Resource
{
	RESOURCE_DECLARATION(ResourceBone);

private:
	ResourceBone();//Constructor is private because Resources need to be created from ModuleResourceManager

	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

private:

	const char * name = nullptr;

};

#endif