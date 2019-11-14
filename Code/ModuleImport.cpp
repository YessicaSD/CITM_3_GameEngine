#include "Application.h"
#include "ModuleImport.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ResourceMesh.h"
#include "ModuleTexture.h"
#include "ModuleResourceManager.h"
#include "Event.h"

#include "Resource.h"
#include "ResourceModel.h"

#include "ModuleFileSystem.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par\par_shapes.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

void AssimpWrite(const char *text, char *data)
{

	std::string tmp_txt = text;
	std::string::const_iterator index = std::remove(tmp_txt.begin(), tmp_txt.end(), '%');
	tmp_txt.erase(index, tmp_txt.end());
	LOG(tmp_txt.c_str());
}

ModuleImport::ModuleImport(const char *name) : Module(true, name)
{
}

bool ModuleImport::Start(JSONFile * config)
{
	LOG("Creating assimp LOG stream");
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = AssimpWrite;
	aiAttachLogStream(&stream);
	return true;
}

//Creates an AssetMesh (our custom format for 3d meshes) from an fbx
bool ModuleImport::ImportModel(const char *path)
{
	const aiScene *scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		std::vector<ResourceMesh *> fbx_meshes;
		std::vector<UID> fbx_meshes_uids;

		std::vector<ResourceTexture *> fbx_textures;
		std::vector<UID> fbx_textures_uids;

		ResourceModelNode model_root_node;
		ResourceModel * resource_model = nullptr;

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh *assimp_mesh = scene->mMeshes[i];
			ResourceMesh *resource_mesh = ImportAssimpMesh(assimp_mesh);
			fbx_meshes.push_back(resource_mesh);
			fbx_meshes_uids.push_back(resource_mesh->GetUID());

			ResourceTexture * resource_texture = ImportFBXTexture(assimp_mesh, scene);
			fbx_textures.push_back(resource_texture);
			fbx_textures_uids.push_back(resource_texture->GetUID());
		}

		resource_model = App->resource_manager->CreateNewResource<ResourceModel>();
		LoadFBXNodes(resource_model, &model_root_node, scene->mRootNode, fbx_meshes_uids, fbx_textures_uids, 0);//If it's itself, it will know it is the root node
		resource_model->SaveFileData();

		last_model_imported = resource_model;

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}

	return true;
}

bool ModuleImport::LoadFBXNodes(ResourceModel * resource_model, ResourceModelNode * model_node, aiNode * node, const std::vector<UID>& meshes, const std::vector<UID>& materials, uint parent_index)
{
	uint curr_index = resource_model->nodes.size();

	model_node->name = (char*)node->mName.C_Str();
	model_node->transform = reinterpret_cast<const float4x4&>(node->mTransformation);
	model_node->parent_index = parent_index;

	if (node->mNumMeshes > 0u)
	{
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			uint index = node->mMeshes[i];
			model_node->mesh_uid = index;
			model_node->material_uid = index;
		}
		//TODO: Create a new ResourceModelNode for each mesh
		resource_model->nodes.push_back(model_node);
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		LoadFBXNodes(resource_model, new ResourceModelNode(), node->mChildren[i], meshes, materials, curr_index);
	}

	return true;
}

ResourceTexture * ModuleImport::ImportFBXTexture(aiMesh * info, const  aiScene* fbx)
{
	aiMaterial* material = fbx->mMaterials[info->mMaterialIndex];

	ResourceTexture * ret = nullptr;

	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString aipath;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aipath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string path = ASSETS_FOLDER + std::string(aipath.data);
			ret = App->texture->LoadTexture(path.c_str());
		}
		else
		{
			LOG("Texture path note found");
		}
	}
	return ret;
}

ResourceMesh *ModuleImport::ImportAssimpMesh(aiMesh *assimp_mesh)
{
	ResourceMesh *resource_mesh = App->resource_manager->CreateNewResource<ResourceMesh>();
	//INFO: We can only do this cast because we know that aiVector3D is 3 consecutive floats
	resource_mesh->LoadVertices(assimp_mesh->mNumVertices, (const float *)assimp_mesh->mVertices);
	resource_mesh->CreateBoundingBox();
	resource_mesh->LoadVerticesNormals(assimp_mesh);
	resource_mesh->LoadFaces(assimp_mesh);
	resource_mesh->CalculateFaceNormals();
	resource_mesh->LoadUVs(assimp_mesh);

	resource_mesh->GenerateVertexNormalsBuffer();
	resource_mesh->GenerateVerticesBuffer();
	resource_mesh->GenerateFacesAndNormalsBuffer();
	resource_mesh->GenerateUVsBuffer();

	resource_mesh->SaveFileData();

	return resource_mesh;
}

ResourceMesh *ModuleImport::ImportParShapeMesh(par_shapes_mesh *mesh)
{
	ResourceMesh *resource_mesh = App->resource_manager->CreateNewResource<ResourceMesh>();

	resource_mesh->LoadVertices(mesh->npoints, mesh->points);
	resource_mesh->CreateBoundingBox();

	//TODO: Get vertices normals
	resource_mesh->LoadFaces(mesh->ntriangles, mesh->triangles);
	resource_mesh->CalculateFaceNormals();
	resource_mesh->LoadUVs(mesh->tcoords);

	resource_mesh->GenerateVerticesBuffer();
	resource_mesh->GenerateFacesAndNormalsBuffer();
	resource_mesh->GenerateUVsBuffer();

	resource_mesh->SaveFileData();

	return resource_mesh;
}

void ModuleImport::CreateGameObjectFromModel(ResourceModel * resource_model, ComponentTransform * parent)
{
	std::vector<GameObject*> model_gameobjects;
	for (uint i = 0u; i < resource_model->nodes.size(); ++i)
	{
		GameObject * new_gameobject = new GameObject(resource_model->nodes[i]->name, nullptr);
		new_gameobject->transform->SetTransform(resource_model->nodes[i]->transform);
		if (resource_model->nodes[i]->mesh_uid != 0)
		{
			ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			component_mesh->mesh = (ResourceMesh*)App->resource_manager->GetResource(resource_model->nodes[i]->mesh_uid);
			//component_mesh->bounding_box->MultiplyByMatrix(new_gameobject->transform->GetGlobalMatrix(), component_mesh->mesh->GetAABB());
			//TODO: Set texture using material uid
		}
		model_gameobjects.push_back(new_gameobject);
	}
	//Parent them
	for (uint i = 0u; i < model_gameobjects.size(); ++i)
	{
		if (resource_model->nodes[i]->parent_index != 0)
		{
			model_gameobjects[i]->transform->SetParent(model_gameobjects[resource_model->nodes[i]->parent_index]->transform);
		}
	}
	model_gameobjects[0]->transform->SetParent(parent);
}

void ModuleImport::CreateGameObjectsFromNodes(aiNode *node, ComponentTransform *parent, std::vector<ResourceMesh *> loaded_meshes, std::vector<ResourceTexture *> &textures)
{
	GameObject *new_gameobject = new GameObject(std::string(node->mName.C_Str()), parent);

	aiVector3D translation, scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	new_gameobject->transform->SetTransform(float3(translation.x, translation.y, translation.z), float3(scaling.x, scaling.y, scaling.z), Quat(rotation.x, rotation.y, rotation.z, rotation.w));
	new_gameobject->transform->Reset();

	if (node->mNumMeshes > 0u)
	{
		//Load the meshes of this GameObject
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			int index = node->mMeshes[i];
			ComponentMesh *component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			component_mesh->mesh = loaded_meshes[index];
			component_mesh->bounding_box->MultiplyByMatrix(new_gameobject->transform->GetGlobalMatrix(), component_mesh->mesh->GetAABB());
			if (textures[index] != nullptr)
			{
				component_mesh->material->SetTexture(textures[index]);
			}
		}
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		CreateGameObjectsFromNodes(node->mChildren[i], new_gameobject->transform, loaded_meshes, textures);
	}
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

void ModuleImport::EventRequest(const Event &event)
{
	if (event.type == Event::EVENT_TYPE::DROPPED_FILE)
	{

		std::string extension;
		App->file_system->GetExtension(event.path, extension);
		if (extension == "fbx" || extension == "FBX")
		{
			//Import mesh onto assets folder
			ImportModel(event.path);
		}
		else if (extension == "dds" || extension == "png" || extension == "jpg")
		{
			//Import texture onto assets folder
			App->texture->LoadTexture(event.path);
		}
		else
		{
			LOG("Can't load this type file");
			return;
		}
		LOG("File dropped %s", event.path);
	}
}