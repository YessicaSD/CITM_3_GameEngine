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
	ModuleTexture(const char * name);
	bool Init(JSONFile * module_file) override;
	bool ImportTexture(const char * path);
	Texture* LoadTexture(const char* path);
	bool CleanUp() override;
	friend class PanelAssets;
	void makeCheckTexture();
};




#endif // !MODULE_TEXTURE

