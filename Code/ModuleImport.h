#ifndef MODULE_IMPORT_H_
#define MODULE_IMPORT_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/assimp/cimport.h"
#include "ModuleScene.h"
#include "AssetTexture.h"

class AssetMesh;
struct aiMesh;
struct aiNode;
class ComponentTransform;

typedef struct par_shapes_mesh_s par_shapes_mesh;

//Module responsible for importing assets into the engine
class ModuleImport : public Module
{
public:
	ModuleImport(const char * name);
	bool Start(JSONFile * module_file) override;
	bool ImportMesh(const char* path);
	bool LoadFBXTexture(aiMesh * info, const aiScene * fbx, std::vector<AssetTexture*>& textures);
	bool CleanUp() override;

	void EventRequest(const Event& event) override;
	AssetMesh* LoadAssimpMesh(aiMesh * assimp_mesh);
	AssetMesh* LoadParShapeMesh(par_shapes_mesh * mesh);
	GameObject * CreateGameObjectWithMesh(std::string name, ComponentTransform * parent, AssetMesh * asset_mesh);
	bool AddMesh(AssetMesh * asset_mesh);
private:
	void CreateGameObjectsFromNodes(aiNode * node, ComponentTransform * parent, std::vector<AssetMesh*> loaded_meshes, std::vector<AssetTexture*>& textures);
	
public:
	AssetTexture * lenna_img_id = nullptr;

private:
	
	std::vector<AssetMesh*> meshes;

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
