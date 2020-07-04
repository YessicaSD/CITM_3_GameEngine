#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__

#include "Resource.h"
#include <map>
#include "../MathGeoLib/include/Math/float4.h"
#include "Color.h"
class ResourceTexture;

class ResourceMaterial : public Resource
{
	RESOURCE_DECLARATION(ResourceMaterial);

public:
	const char * GetTypeString() override;
	void SetColor(std::string name, Color color);
private:
	bool SaveFileData() override;
	bool LoadFileData() override;
	bool ReleaseData() override;

public:
	int i = 0;
	std::map<std::string, Color> colors;
	std::map<std::string, ResourceTexture*> textures;
	std::map<std::string, float> floats;
	std::map<std::string, int> ints;

};

#endif