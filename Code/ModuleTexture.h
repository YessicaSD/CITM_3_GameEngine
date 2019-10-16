#ifndef MODULE_TEXTURE
#define MODULE_TEXTURE

#include "Module.h"
#include <map>

class Texture;

class ModuleTexture : public Module
{
private:
	std::map<std::string, Texture*> textures;

public:  
	ModuleTexture() {};
	bool Init() override;
	Texture* LoadTexture(const char* path);
	bool CleanUp() override;
	friend class PanelAssets;
};




#endif // !MODULE_TEXTURE

