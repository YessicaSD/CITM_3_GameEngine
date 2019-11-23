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
	//INFO: Scan assets folder
	//1. See if they have a .meta
		//else Create .meta for them
	//2. See if they have been edited (dateModified is the same)
		//else import a new resource

	//Assets which cannot be opened show "cannot open this file" in preview window

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
		const char * meta_name = (curr_dir + (*iter)->name + std::string(".meta")).c_str();
		//TODO: It must also get the name of the folder we're in

		std::string extension;
		App->file_system->GetExtension((*iter)->name.c_str(), extension);
		uint type = GetResourceTypeFromExtension(extension);

		//Check if it has a .meta. That means it has been imported already.
		if (App->file_system->FileExists(meta_name))
		{
			//Check that the modified date of the .meta and the file match. That means the file hasn't been modified while the engine was closed.
			JSONFile meta_file;
			meta_file.LoadFile(meta_name);
			int meta_file_dateModified = meta_file.LoadNumber("dateModified");

			int asset_file_dateModified = 0;
			struct stat file_stat;
			if (stat((*iter)->name.c_str(), &file_stat) == 0)
			{
				asset_file_dateModified = file_stat.st_atime;
			}

			if (asset_file_dateModified != meta_file_dateModified)
			{
				//Import the file. Resources force the previous uid.
			}

			//OR if there is a .meta but there isn't a RESOURCES_FOLDER
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
				LOG("This format cannot be imported.");
			}
		}
	}
	for (auto iter = dir->dirs.begin(); iter != dir->dirs.end(); ++iter)
	{
		ImportAssetsRecursively((*iter), curr_dir + (*iter)->name);
	}
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
	RELEASE(asset_dir);
	return true;
}

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
		if (extension != "meta")
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