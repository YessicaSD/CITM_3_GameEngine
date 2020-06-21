#ifndef __MODULE_IMPORT_TEXTURE_H__
#define __MODULE_IMPORT_TEXTURE_H__

#include "Module.h"
#include <map>
#include "../DevIL/include/il.h"
#include "Resource.h"

class ResourceTexture;

struct TextureImportOptions
{
	ILint compression = IL_DXT5;
};

class ModuleImportTexture : public Module
{
public:
	ModuleImportTexture(const char * name);
	bool Init(JSONFile * module_file) override;
	ResourceTexture* ImportTexture(const char * path, UID forced_uid = INVALID_RESOURCE_UID);
	void SaveTextureMeta(ResourceTexture * resource_texture, const char * asset_path);
	//void ImportCheckerTexture();
	
	friend class PanelAssets;
};

#endif // !MODULE_TEXTURE

