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
class ResourceTexture;
struct aiMesh;
struct aiNode;
class ComponentTransform;
class ResourceModel;

typedef struct par_shapes_mesh_s par_shapes_mesh;

//Module responsible for importing assets into the engine
class ModuleImport : public Module
{
public:
	ModuleImport(const char * name);
	bool Start(JSONFile * module_file) override;
	ResourceModel * ImportModel(const char* path);
	bool LoadFBXNodes(ResourceModel * resource_model, ResourceModelNode * model_node, aiNode * node, const std::vector<UID>& meshes, const std::vector<UID>& materials, uint parent_index);
	bool CleanUp() override;

	void EventRequest(const Event& event) override;
	ResourceMesh* ImportAssimpMesh(aiMesh * assimp_mesh);
	ResourceTexture * ImportFBXTexture(aiMesh * info, const aiScene * fbx);
	ResourceMesh* ImportParShapeMesh(par_shapes_mesh * mesh);
	void CreateGameObjectFromModel(ResourceModel * resource_model, ComponentTransform * parent);

private:
	void CreateGameObjectsFromNodes(aiNode * node, ComponentTransform * parent, std::vector<ResourceMesh*> loaded_meshes, std::vector<ResourceTexture*>& textures);

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
