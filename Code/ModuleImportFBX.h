#ifndef MODULE_IMPORT_FBX_H_
#define MODULE_IMPORT_FBX_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/assimp/cimport.h"



class Mesh;

struct Scene
{
	std::vector<Mesh*> array_mesh;
};
class ModuleImportFBX : public Module
{
private:
	aiLogStream stream;

	std::vector<Scene*> array_scene;
public:
	bool Start() override;
	
	bool LoadMesh(const char* path);

	bool CleanUp() override;

	update_status PostUpdate() override;

	

};
#endif // !MODULE_IMPORT_FBX_H_
