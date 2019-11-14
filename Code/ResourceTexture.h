#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

typedef unsigned int uint;
#include <string>

#include "Resource.h"

class ResourceTexture : public Resource
{
private:
	ResourceTexture() {
		path = "";
	};
public:
	~ResourceTexture();

	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

	//void GenerateTexture();
	//void Render();

public:
	uint buffer_id = 0u, size = 0u;
	uint width = 0u, height = 0u;
	std::string path;

	friend class ModuleResourceManager;
};




#endif // !TEXTURE_H_

