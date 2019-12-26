#ifndef __MODULE_IMPORT_MODEL_H_
#define __MODULE_IMPORT_MODEL_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/assimp/cimport.h"
#include "ModuleScene.h"
#include "ResourceTexture.h"


class ResourceMesh;
class ModelNode;
struct aiMesh;
struct aiMaterial;
struct aiNode;
class ComponentTransform;
class ResourceModel;

typedef struct par_shapes_mesh_s par_shapes_mesh;
enum aiPostProcessSteps;

struct ModelImportOptions
{
	aiPostProcessSteps post_process_steps;
};

//Module responsible for importing assets into the engine
class ModuleImportModel : public Module
{
public:
	ModuleImportModel(const char * name);
	bool Start(JSONFile * module_file) override;
	ResourceModel * ImportModel(
		const char* path,
		UID model_uid = INVALID_RESOURCE_UID,
		std::vector<UID> & meshes_uids = std::vector<UID>(),
		std::vector<UID> & textures_uids = std::vector<UID>(),
		std::vector<UID>& animation_uids = std::vector<UID>(),
		std::vector<UID>& bones_uids = std::vector<UID>());
	bool CleanUp() override;
	void EventRequest(const Event& event) override;

	void CreateGameObjectFromModel(ResourceModel * resource_model, ComponentTransform * parent);

private:
	void SaveModelMeta(ResourceModel * resource_model, const char * asset_path);
	bool ImportFBXNodes(ResourceModel * resource_model, ModelNode * model_node, aiNode * node, const std::vector<uint> mesh_texture_idxs, uint parent_index);
	ResourceTexture * ImportFBXTexture(const  aiMaterial * material, std::vector<UID> & uids, const char * asset_path);
	void LoadModelMeta(ResourceModel * model, const char * meta_path);

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
