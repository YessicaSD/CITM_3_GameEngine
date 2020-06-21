#include "ModuleImportBone.h"
#include "ResourceBone.h"
#include "Application.h"
#include "ModuleResourceManager.h"
#include "../Assimp/include/mesh.h"
#include "ResourceModel.h"

ModuleImportBone::ModuleImportBone(const char * name) : Module(true, name)
{
}

ResourceBone* ModuleImportBone::ImportBone(aiBone * assimp_bone, UID uid, const char * asset_path)
{
	Timer import_timer;

	ResourceBone* resource_bone = App->resource_manager->CreateResource<ResourceBone>();
	resource_bone->asset_source = asset_path;

	//Load the data onto the variables
	const char * bone_name = assimp_bone->mName.C_Str();
	resource_bone->name = new char[NODE_NAME_SIZE];
	memset(resource_bone->name, NULL, NODE_NAME_SIZE);
	strcpy(resource_bone->name, bone_name);

	resource_bone->offset_matrix = reinterpret_cast<const float4x4&>(assimp_bone->mOffsetMatrix);

	resource_bone->num_weights = assimp_bone->mNumWeights;

	resource_bone->weights = new VertexWeigth[assimp_bone->mNumWeights];
	//TODO: See if we can do a memcpy instead of iterating
	for (int i = 0; i < resource_bone->num_weights; ++i)
	{
		resource_bone->weights[i].vertex_id = assimp_bone->mWeights[i].mVertexId;
		resource_bone->weights[i].weigth = assimp_bone->mWeights[i].mWeight;
	}

	resource_bone->SaveFileData();

	LOG("Success importing bone with uid: %llu in: %i ms.", resource_bone->GetUID(), import_timer.Read());

	return resource_bone;
}
