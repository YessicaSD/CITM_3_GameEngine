#ifndef MODULE_TEXTURE
#define MODULE_TEXTURE

#include "Module.h"
#include <map>
class AssetTexture;

class ModuleTexture : public Module
{
private:
	std::map<std::string, AssetTexture*> textures;

public:
	ModuleTexture(const char * name);
	bool Init(JSONFile * module_file) override;
	bool ImportTexture(const char * path);
	AssetTexture* LoadTexture(const char* path);
	bool CleanUp() override;
	friend class PanelAssets;
	void CreateCheckerTexture();
};




#endif // !MODULE_TEXTURE

