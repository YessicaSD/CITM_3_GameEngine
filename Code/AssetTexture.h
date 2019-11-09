#ifndef __ASSET_TEXTURE_H__
#define __ASSET_TEXTURE_H__
typedef unsigned int uint;
#include <string>
class AssetTexture
{
public:
	AssetTexture() {
		path = "";
	};
	~AssetTexture();

	//void GenerateTexture();
	//void Render();

public:
	int buffer_id = -1, size = 0;
	uint width = 0, height = 0;
	std::string path;
};




#endif // !TEXTURE_H_

