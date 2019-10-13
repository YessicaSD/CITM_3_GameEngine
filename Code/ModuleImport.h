#ifndef MODULE_IMPORT_H_
#define MODULE_IMPORT_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/assimp/cimport.h"
#include "ModuleScene.h"


class ComponentMesh;
struct aiMesh;

struct AssimpScene
{
	std::vector<ComponentMesh*> assimp_meshes;
};

//Module responsible for importing assets into the engine

class ModuleImport : public Module
{
protected:
	aiLogStream stream;
	std::vector<AssimpScene*> array_scene;
public:
	bool Start() override;
	
	bool LoadMesh(const char* path);

	void LoadVertices(ComponentMesh * component_mesh, aiMesh * assimp_mesh);

	bool CleanUp() override;

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
