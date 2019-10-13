#ifndef MODULE_TEXTURE
#define MODULE_TEXTURE

#include "Module.h"
#include <map>

class Texture;

class ModuleTexture : public Module
{
private:
	std::map<const char* , Texture*> textures;

public:  
	ModuleTexture() {};
	bool Init() override;
	Texture* LoadTexture(const char* path);
};




#endif // !MODULE_TEXTURE

