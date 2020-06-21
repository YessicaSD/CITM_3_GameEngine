#ifndef __MODULE_IMPORT_MODEL_H_
#define __MODULE_IMPORT_MODEL_H_

#include "Module.h"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF
#include "Assimp/include/cimport.h"
#include "ModuleScene.h"
#include "ResourceTexture.h"
#include <unordered_set>


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

	GameObject * CreateGameObjectFromModel(ResourceModel * resource_model, ComponentTransform * parent);

private:
	void SaveModelMeta(ResourceModel * resource_model, const char * asset_path);
	uint GetGeneration(ResourceModel * resource_model, int node_idx);
	bool ImportModelNodes(ResourceModel * resource_model, aiNode * node, const std::vector<UID> & mesh_texture_idxs, uint parent_index, float4x4 curr_transformation);
	ResourceTexture * ImportModelTexture(const  aiMaterial * material, std::vector<UID> & uids, const char * asset_path);
	void LoadModelMeta(ResourceModel * model, const char * meta_path);

	friend ModuleScene;
};
#endif // !MODULE_IMPORT_FBX_H_
