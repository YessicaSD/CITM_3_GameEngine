#ifndef TEXTURE_H_
#define TEXTURE_H_
typedef unsigned int uint;
#include <string>
class Texture
{
public:
	Texture() {
		path = "";
	};
	~Texture();

	//void GenerateTexture();
	//void Render();

public:
	int buffer_id = -1, size = 0;
	

	uint width = 0, height = 0;
	std::string path;
};




#endif // !TEXTURE_H_

