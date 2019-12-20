#include "ModuleResourceManager.h"
#include "ModuleRandom.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleTexture.h"
#include "ModuleImport.h"

#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceMaterial.h"

ModuleResourceManager::ModuleResourceManager(const char * name) : Module(true, name)
{
}

bool ModuleResourceManager::Start(JSONFile * module_file)
{
	check_assets_interval = 1.f;
	//TODO: Assets which cannot be opened show "cannot open this file" in preview window

	asset_dir = new AssetDir();
	asset_dir->name = ASSETS_FOLDER;
	asset_dir->full_path = ASSETS_FOLDER;
	CreateAssetTree(asset_dir);
	StartCheckAssets(asset_dir);
	check_assets_timer.Start();

	return true;
}

//INFO: Only executed when the engine starts. It's recursive function.
//TODO: Make it not recursive
void ModuleResourceManager::StartCheckAssets(AssetDir* dir)
{
	for (auto iter = dir->assets.begin(); iter != dir->assets.end(); ++iter)
	{
		std::string meta_path = (*iter)->full_path + std::string(".") + std::string(META_EXTENSION);

		std::string extension;
		App->file_system->GetExtension((*iter)->name.c_str(), extension);
		uint type = GetResourceTypeFromExtension(extension);

		//Check if it has a .meta. That means it has been imported already.
		if (App->file_system->FileExists(meta_path.c_str()))
		{
			JSONFile meta_file;
			meta_file.LoadFile(meta_path);

			if (HasBeenModified(meta_file, (*iter)->full_path.c_str())
				|| MissingResources(meta_file, type))
			{
				ReImportResources(meta_file, type, (*iter));
			}
			else
			{
				CreateResourcesInMap(type, meta_file, (*iter));
			}
		}
		else
		{
			ImportResource(type, (*iter)->full_path.c_str());
		}
	}
	for (auto iter = dir->dirs.begin(); iter != dir->dirs.end(); ++iter)
	{
		StartCheckAssets((*iter));
	}
}

void ModuleResourceManager::UpdateCheckAssets(AssetDir* dir)
{
	for (auto iter = dir->assets.begin(); iter != dir->assets.end(); ++iter)
	{
		std::string meta_path = (*iter)->full_path + std::string(".") + std::string(META_EXTENSION);

		std::string extension;
		App->file_system->GetExtension((*iter)->name.c_str(), extension);
		uint type = GetResourceTypeFromExtension(extension);

		//Check if it has a .meta. That means it has been imported already.
		if (App->file_system->FileExists(meta_path.c_str()))
		{
			JSONFile meta_file;
			meta_file.LoadFile(meta_path);

			if (HasBeenModified(meta_file, (*iter)->full_path.c_str()))
			{
				ReImportResources(meta_file, type, (*iter));
			}
		}
		else
		{
			ImportResource(type, (*iter)->full_path.c_str());
		}
	}
	for (auto iter = dir->dirs.begin(); iter != dir->dirs.end(); ++iter)
	{
		UpdateCheckAssets((*iter));
	}
}

//Import resources which have changed or deleted their resources on the resources folder.
//First delete the existing ones and then import them forcing the previous uids.
void ModuleResourceManager::ReImportResources(JSONFile &meta_file, const uint &type, AssetFile * asset_file)
{
	const char * uid_string = meta_file.LoadText("resourceUID", "0");
	UID uid = strtoull(uid_string, nullptr, 10);
	//Import the file. Resources force the previous uid.
	if (type == ResourceModel::type)
	{
		//INFO: Delete the previous resources
		App->file_system->Remove((std::string(RESOURCES_MODEL_FOLDER) + uid_string + "." + MODEL_EXTENSION).c_str());
		resources.erase(uid);

		std::vector<UID>meshes_uids;
		DeleteDependantResources(meshes_uids, "exportedMeshes", &meta_file, RESOURCES_MESH_FOLDER, MESH_EXTENSION);

		std::vector<UID>textures_uids;
		DeleteDependantResources(textures_uids, "exportedTextures", &meta_file, RESOURCES_TEXTURES_FOLDER, TEXTURE_EXTENSION);

		std::vector<UID>animation_uids;
		DeleteDependantResources(animation_uids, "exportedAnimations", &meta_file, RESOURCES_ANIMATION_FOLDER, ANIMATION_EXTENSION);

		//INFO: Generate new resources using the previous uids
		App->import->ImportModel(asset_file->full_path.c_str(), uid, meshes_uids, textures_uids);
	}
	else if (type == ResourceTexture::type)
	{
		//INFO: Delete the previous resources
		App->file_system->Remove((std::string(RESOURCES_TEXTURES_FOLDER) + uid_string + "." + TEXTURE_EXTENSION).c_str());
		resources.erase(uid);

		App->texture->ImportTexture(asset_file->full_path.c_str(), uid);
	}
	else
	{
		LOG("This format is unsupported.");
	}
}

void ModuleResourceManager::CreateResourcesInMap(const uint &type, JSONFile &meta_file, AssetFile * asset_file)
{
	if (type == ResourceModel::type)
	{
		ResourceModel * resource_model = CreateResource<ResourceModel>(meta_file.LoadUID("resourceUID"));
		resource_model->asset_source = asset_file->full_path;

		std::vector<UID>meshes_uids;
		meta_file.LoadUIDVector("exportedMeshes", meshes_uids);
		for (auto mesh_iter = meshes_uids.begin(); mesh_iter != meshes_uids.end(); ++mesh_iter)
		{
			ResourceMesh * resource_mesh = CreateResource<ResourceMesh>((*mesh_iter));
			resource_mesh->asset_source = asset_file->full_path;
		}

		std::vector<UID>textures_uids;
		meta_file.LoadUIDVector("exportedTextures", textures_uids);
		for (auto texture_iter = textures_uids.begin(); texture_iter != textures_uids.end(); ++texture_iter)
		{
			ResourceTexture * resource_texture = CreateResource<ResourceTexture>((*texture_iter));
			resource_texture->asset_source = asset_file->full_path;
		}
	}
	else if (type == ResourceTexture::type)
	{
		ResourceTexture * resource_texture = CreateResource<ResourceTexture>(meta_file.LoadUID("resourceUID"));
		resource_texture->asset_source = asset_file->full_path;
	}
	else
	{
		LOG("This format is unsupported.");
	}
}

//Import resource without forcing the uid
void ModuleResourceManager::ImportResource(const uint type, const char * path)
{
	if (type == ResourceModel::type)
	{
		App->import->ImportModel(path);
	}
	else if (type == ResourceTexture::type)
	{
		App->texture->ImportTexture(path);
	}
	//else
	//{
	//	LOG("This format is unsupported.");
	//}
}

//Check that the modified date of the .meta and the file match. That means the file hasn't been modified while the engine was closed.
bool ModuleResourceManager::HasBeenModified(JSONFile &meta_file, const char * file)
{
	int meta_file_dateModified = meta_file.LoadNumber("dateModified");
	int asset_file_dateModified = 0;
	struct stat file_stat;
	if (stat(file, &file_stat) == 0)
	{
		asset_file_dateModified = file_stat.st_atime;
	}
	return asset_file_dateModified != meta_file_dateModified;
}

bool ModuleResourceManager::MissingResources(JSONFile &meta_file, uint type)
{
	bool ret = false;
	//Check that the resources specified in the .meta are indeed created in the Resources folder.

	UID resource_uid = LoadUID(&meta_file);
	char buffer[UID_DIGITS];
	sprintf_s(buffer, "%020llu", resource_uid);

	if (type == ResourceModel::type)
	{
		if (!App->file_system->FileExists((std::string(RESOURCES_MODEL_FOLDER) + buffer + "." + MODEL_EXTENSION).c_str()))
		{
			ret = true;
		}
		//TODO: Check if it lacks meshes or textures
	}
	else if (type == ResourceTexture::type)
	{
		if (!App->file_system->FileExists((std::string(RESOURCES_TEXTURES_FOLDER) + buffer + "." + TEXTURE_EXTENSION).c_str()))
		{
			ret = true;
		}
	}
	else
	{
		LOG("This format is unsupported.");
	}
	return ret;
}

//ModuleGUI
//+ Refresh button to check for new resources

update_status ModuleResourceManager::PreUpdate()
{
	if (check_assets_timer.ReadSec() > check_assets_interval)
	{
		DeleteAssetTree(asset_dir);
		asset_dir = new AssetDir();
		asset_dir->name = ASSETS_FOLDER;
		asset_dir->full_path = ASSETS_FOLDER;
		CreateAssetTree(asset_dir);
		UpdateCheckAssets(asset_dir);
		check_assets_timer.Start();
	}
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	DeleteAssetTree(asset_dir);
	//RELEASE(asset_dir);
	return true;
}

//This could use a template too to avoid casting
Resource * ModuleResourceManager::GetResource(UID uid)
{
	Resource * resource = nullptr;
	auto iter = resources.find(uid);
	if (iter != resources.end())
	{
		resource = iter->second;
	}
	else
	{
		LOG("Error: The uid specified is not in the resources map.");
	}
	return resource;
}

UID ModuleResourceManager::GenerateNewUID()
{
	return App->random->RandomUID();
}

//TODO: Make a function directly on file system that returns files without .meta and returns class AssetFiles and class Dir instead of std::string
//don't abstract it over an existing one, we end up with 2 lists for each
//INFO: Recursive function
void ModuleResourceManager::CreateAssetTree(AssetDir * dir)
{
	std::vector<std::string> file_list;
	std::vector<std::string> dir_list;

	App->file_system->GetFilesAndDirs(dir->full_path.c_str(), file_list, dir_list);

	//Add files
	for (auto iter = file_list.begin(); iter != file_list.end(); ++iter)
	{
		std::string extension;
		App->file_system->SplitFilePath((*iter).c_str(), nullptr, nullptr, &extension);
		//Ignore .meta
		if (extension != META_EXTENSION)
		{
			AssetFile * new_asset = new AssetFile();
			new_asset->name = (*iter);
			new_asset->full_path = dir->full_path + (*iter);
			dir->assets.push_back(new_asset);
		}
	}

	//Add dirs
	for (auto iter = dir_list.begin(); iter != dir_list.end(); ++iter)
	{
		AssetDir * new_dir = new AssetDir();
		new_dir->name = (*iter);
		new_dir->full_path = dir->full_path + (*iter) + "/";
		CreateAssetTree(new_dir);
		dir->dirs.push_back(new_dir);
	}
}

//INFO: Recursive function
void ModuleResourceManager::DeleteAssetTree(AssetDir * dir)
{
	for (auto iter = dir->assets.begin(); iter != dir->assets.end(); ++iter)
	{
		delete((*iter));
	}

	for (auto iter = dir->dirs.begin(); iter != dir->dirs.end(); ++iter)
	{
		DeleteAssetTree((*iter));
	}
	delete(dir);
}

uint ModuleResourceManager::GetResourceTypeFromExtension(const std::string & extension)
{
	if (extension == "fbx" || extension == "FBX")
	{
		return ResourceModel::type;
	}
	else if (extension == "dds" || extension == "png" || extension == "jpg" || extension == "tga")
	{
		return ResourceTexture::type;
	}
}

UID ModuleResourceManager::LoadUID(JSONFile * meta_file) const
{
	const char * aux_uid = meta_file->LoadText("resourceUID", "0");
	return strtoull(aux_uid, nullptr, 10);
}

void ModuleResourceManager::SaveUID(JSONFile * meta_file, const UID & uid) const
{
	char buffer[UID_DIGITS];
	sprintf_s(buffer, "%020llu", uid);
	meta_file->SaveText("resourceUID", buffer);
}

void ModuleResourceManager::SaveUIDArray(const std::vector<UID> & uid_vector, char * name, JSONFile * meta_file) const
{
	uint count = uid_vector.size();
	const char ** uid_string_vector = new const char*[count];
	for (int i = 0; i < count; ++i)
	{
		char * buffer = new char[UID_DIGITS];
		sprintf_s(buffer, UID_DIGITS, "%020llu", uid_vector[i]);
		uid_string_vector[i] = buffer;
	}
	meta_file->SaveTextArray(name, uid_string_vector, count);
	for (int i = count - 1; i >= 0; --i)
	{
		delete[]uid_string_vector[i];
	}
	delete[]uid_string_vector;
}

void ModuleResourceManager::DeleteDependantResources(std::vector<UID> & uids, const char * name, JSONFile * meta_file, const char * folder, const char * extension)
{
	std::vector<const char *>uids_string;
	meta_file->LoadTextVector(name, uids_string);
	uint size = uids_string.size();
	uids.reserve(size);
	for (int i = 0; i < size; ++i)
	{
		uids.push_back(strtoull(uids_string[i], nullptr, 10));
	}
	for (int i = 0; i < size; ++i)
	{
		App->file_system->Remove((std::string(folder) + uids_string[i] + "." + extension).c_str());
		resources.erase(uids[i]);
	}
}