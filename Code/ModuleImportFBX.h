#ifndef MODULE_IMPORT_FBX_H_
#define MODULE_IMPORT_FBX_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/assimp/cimport.h"
#include "ModuleScene.h"


class Mesh;

struct Scene
{
	std::vector<Mesh*> array_mesh;
};

class ModuleImportFBX : public Module
{
protected:
	aiLogStream stream;
	std::vector<Scene*> array_scene;
public:
	bool Start() override;
	
	bool LoadMesh(const char* path);

	bool CleanUp() override;

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
