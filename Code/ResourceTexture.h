#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

typedef unsigned int uint;
#include <string>

#include "Resource.h"

class ResourceTexture : public Resource
{
	RESOURCE_DECLARATION(ResourceTexture)

public:
	~ResourceTexture();

private:
	ResourceTexture();

	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

public:
	uint buffer_id = 0u;
	uint size = 0u;
	uint width = 0u;
	uint height = 0u;

	friend class ModuleTexture;
	friend class ModuleResourceManager;
};




#endif // !TEXTURE_H_

