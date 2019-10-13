#ifndef TEXTURE_H_
#define TEXTURE_H_

typedef unsigned int uint;
class Texture
{
public:
	Texture() {};
	~Texture() {};

	//void GenerateTexture();
	//void Render();

public:
	int buffer_id = -1;
	uint width = 0, height = 0;
};




#endif // !TEXTURE_H_

