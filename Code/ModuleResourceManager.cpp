#include "ModuleResourceManager.h"
#include "ModuleRandom.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleTexture.h"
#include "ModuleImport.h"

#include "ResourceModel.h"
#include "ResourceTexture.h"

ModuleResourceManager::ModuleResourceManager(const char * name) : Module(true, name)
{
}

bool ModuleResourceManager::Start(JSONFile * module_file)
{
	//TODO: Assets which cannot be opened show "cannot open this file" in preview window

	asset_dir = new AssetDir();
	asset_dir->name = ASSETS_FOLDER;
	FillAssetTreeRecursive(asset_dir);
	ImportAssetsRecursively(asset_dir, std::string(ASSETS_FOLDER));

	//DeleteTreeRecursive(App->resource_manager->asset_dir);

	return true;
}

//TODO: Make it not recursive
void ModuleResourceManager::ImportAssetsRecursively(AssetDir* dir, std::string curr_dir)
{
	for (auto iter = dir->assets.begin(); iter != dir->assets.end(); ++iter)
	{
		std::string meta_path = curr_dir + (*iter)->name + std::string(".") + std::string(META_EXTENSION);
		//TODO: It must also get the name of the folder we're in

		std::string extension;
		App->file_system->GetExtension((*iter)->name.c_str(), extension);
		uint type = GetResourceTypeFromExtension(extension);

		//Check if it has a .meta. That means it has been imported already.
		if (App->file_system->FileExists(meta_path.c_str()))
		{
			JSONFile meta_file;
			meta_file.LoadFile(meta_path);

			if (IsFileModified(meta_file, (*iter)->name.c_str())
				|| MissingResources(meta_file, type))
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
					
					//INFO: Generate new resources using the previous uids
					App->import->ImportModel((curr_dir + (*iter)->name).c_str(), uid, meshes_uids, textures_uids);
				}
				else if (type == ResourceTexture::type)
				{
					//INFO: Delete the previous resources
					App->file_system->Remove((std::string(RESOURCES_TEXTURES_FOLDER) + uid_string + "." + TEXTURE_EXTENSION).c_str());
					resources.erase(uid);

					App->texture->ImportTexture((curr_dir + (*iter)->name).c_str(), uid);
				}
				else
				{
					LOG("This format is unsupported.");
				}
			}
			else
			{
				//TODO: Load its resources to the map
				//TODO: Put asset_path into the already created resources
			}
		}
		else
		{
			//Import the file. Resources don't force uid
			if (type == ResourceModel::type)
			{
				App->import->ImportModel((curr_dir + (*iter)->name).c_str());
			}
			else if (type == ResourceTexture::type)
			{
				App->texture->ImportTexture((curr_dir + (*iter)->name).c_str());
			}
			else
			{
				LOG("This format is unsupported.");
			}
		}
	}
	for (auto iter = dir->dirs.begin(); iter != dir->dirs.end(); ++iter)
	{
		ImportAssetsRecursively((*iter), curr_dir + (*iter)->name);
	}
}

//Check that the modified date of the .meta and the file match. That means the file hasn't been modified while the engine was closed.
bool ModuleResourceManager::IsFileModified(JSONFile &meta_file, const char * file)
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
		if (!App->file_system->FileExists((std::string(RESOURCES_MODEL_FOLDER) + std::string(buffer)).c_str()))
		{
			ret = true;
		}
		//TODO: Check if it lacks meshes or textures
	}
	else if (type == ResourceTexture::type)
	{
		if (!App->file_system->FileExists((std::string(RESOURCES_TEXTURES_FOLDER) + std::string(buffer)).c_str()))
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
	//TODO: Every second check for new assets and import them
	//TODO: Also check for changes
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	DeleteTreeRecursive(asset_dir);
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
void ModuleResourceManager::FillAssetTreeRecursive(AssetDir * dir)
{
	std::vector<std::string> file_list;
	std::vector<std::string> dir_list;

	App->file_system->GetFilesAndDirs(dir->name.c_str(), file_list, dir_list);

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
			dir->assets.push_back(new_asset);
		}
	}

	//Add dirs
	for (auto iter = dir_list.begin(); iter != dir_list.end(); ++iter)
	{
		AssetDir * new_dir = new AssetDir();
		new_dir->name = dir->name + (*iter);
		FillAssetTreeRecursive(new_dir);
		dir->dirs.push_back(new_dir);
	}
}

void ModuleResourceManager::DeleteTreeRecursive(AssetDir * dir)
{
	for (auto iter = dir->assets.begin(); iter != dir->assets.end(); ++iter)
	{
		delete((*iter));
	}

	for (auto iter = dir->dirs.begin(); iter != dir->dirs.end(); ++iter)
	{
		DeleteTreeRecursive((*iter));
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
		sprintf_s(buffer, UID_DIGITS, "%20llu", uid_vector[i]);
		uid_string_vector[i] = buffer;
	}
	meta_file->SaveTextArray(name, uid_string_vector, count);
	for (int i = count - 1; i >= 0; --i)
	{
		delete[]uid_string_vector[i];
	}
	delete[]uid_string_vector;
}

void ModuleResourceManager::DeleteDependantResources(std::vector<UID> uids, const char * name, JSONFile * meta_file, const char * folder, const char * extension)
{
	std::vector<const char *>meshes_uids_string;
	meta_file->LoadTextVector(name, meshes_uids_string);
	uint size = meshes_uids_string.size();
	std::vector<UID>meshes_uids;
	meshes_uids.reserve(size);
	for (int i = 0; i < size; ++i)
	{
		meshes_uids.push_back(strtoull(meshes_uids_string[i], nullptr, 10));
	}
	for (int i = 0; i < size; ++i)
	{
		App->file_system->Remove((std::string(folder) + meshes_uids_string[i] + "." + extension).c_str());
		resources.erase(meshes_uids[i]);
	}
}