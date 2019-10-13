#ifndef MODULE_IMPORT_H_
#define MODULE_IMPORT_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/assimp/cimport.h"
#include "ModuleScene.h"

class AssetMesh;
struct aiMesh;
struct aiNode;
class ComponentTransform;

struct AssimpScene
{
	std::vector<AssetMesh*> assimp_meshes;
};

//Module responsible for importing assets into the engine

class ModuleImport : public Module
{

public:
	bool Start() override;
	bool LoadMesh(const char* path);
	bool CleanUp() override;

private:
	void LoadFaces(aiMesh * assimp_mesh, AssetMesh * &mesh_component, AssimpScene * new_scene);
	void LoadVertices(AssetMesh * component_mesh, aiMesh * assimp_mesh);
	void CreateGameObjectsFromNodes(aiNode * node, ComponentTransform * parent);

private:
	aiLogStream stream;
	std::vector<AssimpScene*> array_scene;

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
