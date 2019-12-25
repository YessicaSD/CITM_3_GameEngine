#ifndef __MODULE_IMPORT_BONE_H__
#define __MODULE_IMPORT_BONE_H__

#include "Module.h"

struct aiBone;

class ModuleImportBone : public Module
{
public:
	ModuleImportBone(const char *name);
	bool ImportBone(aiBone * assimp_bone, UID uid, const char * asset_path);
};

#endif