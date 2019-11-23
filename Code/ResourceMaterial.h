#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__

#include "Resource.h"

class ResourceMaterial : public Resource
{
	RESOURCE_DECLARATION(ResourceMaterial);

public:
	int i = 0;
	//TODO: Look what information a material holds in assimp
};

#endif