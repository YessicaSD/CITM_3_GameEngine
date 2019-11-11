#ifndef MODULE_IMPORT_H_
#define MODULE_IMPORT_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/assimp/cimport.h"
#include "ModuleScene.h"
#include "ResourceTexture.h"

class ResourceMesh;
class ResourceModelNode;
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
	bool ImportModel(const char* path);
	bool LoadFBXNodes(ResourceModelNode * asset_mesh_node, aiNode * node, const std::vector<UID>& meshes, const std::vector<UID>& materials);
	bool SaveModelCustomFormat(const std::vector<ResourceMesh*>& meshes, const std::vector<ResourceTexture*>& textures, const ResourceModelNode root_node);
	bool ImportFBXTexture(aiMesh * info, const aiScene * fbx, std::vector<ResourceTexture*>& textures);
	bool CleanUp() override;

	void EventRequest(const Event& event) override;
	ResourceMesh* ImportAssimpMesh(aiMesh * assimp_mesh);
	ResourceMesh* LoadParShapeMesh(par_shapes_mesh * mesh);
	GameObject * CreateGameObjectWithMesh(std::string name, ComponentTransform * parent, ResourceMesh * asset_mesh);
	bool AddMesh(ResourceMesh * asset_mesh);
private:
	void CreateGameObjectsFromNodes(aiNode * node, ComponentTransform * parent, std::vector<ResourceMesh*> loaded_meshes, std::vector<ResourceTexture*>& textures);
	
public:
	ResourceTexture * lenna_img_id = nullptr;

private:
	
	std::vector<ResourceMesh*> meshes;

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
