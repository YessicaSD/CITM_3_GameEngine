#ifndef MODULE_IMPORT_H_
#define MODULE_IMPORT_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/assimp/cimport.h"
#include "ModuleScene.h"
#include "Texture.h"
class AssetMesh;
struct aiMesh;
struct aiNode;
class ComponentTransform;

//Module responsible for importing assets into the engine

class ModuleImport : public Module
{

public:
	bool Start() override;
	bool LoadMesh(const char* path);
	bool CleanUp() override;
	Texture* lenna_img_id = 0;
	void EventRequest(const Event& event) override;
private:
	void CreateGameObjectsFromNodes(aiNode * node, ComponentTransform * parent, std::vector<AssetMesh*> loaded_meshes);

private:
	aiLogStream stream;
	std::vector<AssetMesh*> meshes;

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
