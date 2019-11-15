#ifndef MODULE_TEXTURE
#define MODULE_TEXTURE

#include "Module.h"
#include <map>
class ResourceTexture;

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

