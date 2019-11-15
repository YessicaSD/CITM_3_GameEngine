#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

typedef unsigned int uint;
#include <string>

#include "Resource.h"

class ResourceTexture : public Resource
{
public:
	~ResourceTexture();

private:
	ResourceTexture() {
		path = "";
	};

	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

public:
	uint buffer_id = 0u, size = 0u;
	uint width = 0u, height = 0u;
	std::string path;

	friend class ModuleTexture;
	friend class ModuleResourceManager;
};




#endif // !TEXTURE_H_

