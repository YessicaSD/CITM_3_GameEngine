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
	int buffer_id = -1, size = 0;
	uint width = 0, height = 0;
	std::string path;

	friend class ModuleResourceManager;
};




#endif // !TEXTURE_H_

