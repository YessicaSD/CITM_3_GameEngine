#ifndef MODULE_TEXTURE
#define MODULE_TEXTURE

#include "Module.h"
#include <map>
#include "DevIL/include/il.h"

class ResourceTexture;

struct TextureImportOptions
{
	ILint compression = IL_DXT5;
};

class ModuleTexture : public Module
{
public:
	ModuleTexture(const char * name);
	bool Init(JSONFile * module_file) override;
	ResourceTexture* ImportTexture(const char * path);
	void CreateCheckerTexture();
	
	friend class PanelAssets;
};

#endif // !MODULE_TEXTURE

