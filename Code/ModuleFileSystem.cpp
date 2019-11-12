#include "ModuleFileSystem.h"
#include "Module.h"
#include "SDL/include/SDL_filesystem.h"
#include "Globals.h"

#include "Assimp/include/cfileio.h"

#include "AssimpIO.h"
#include "BassIO.h"

#include "PhysFS/include/physfs.h"
#pragma comment (lib, "PhysFS/libx86/physfs.lib")


ModuleFileSystem::ModuleFileSystem(const char * name) : Module (true, name)
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	const char * game_path = ASSETS_FOLDER;

	// workaround VS string directory mess
	AddPath(".");

	if (0 && game_path != nullptr)
	{
		AddPath(game_path);
	}

	// Dump list of paths
	LOG("FileSystem Operations base is [%s] plus:", GetBasePath());
	const char * read_paths = GetReadPaths();
	LOG(read_paths);

	// enable us to write in the game's dir area
	if (PHYSFS_setWriteDir(".") == 0)
	{
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}

	// Make sure standard paths exist
	const char* dirs[] = {
		SETTINGS_FOLDER, ASSETS_FOLDER, RESOURCES_FOLDER,
		RESOURCES_AUDIO_FOLDER, RESOURCES_MESH_FOLDER,
		RESOURCES_MATERIAL_FOLDER, RESOURCES_SCENE_FOLDER, RESOURCES_MODEL_FOLDER,
		RESOURCES_TEXTURES_FOLDER, RESOURCES_ANIMATION_FOLDER, RESOURCES_STATE_MACHINE_FOLDER,
	};

	for (uint i = 0; i < sizeof(dirs) / sizeof(const char*); ++i)
	{
		if (PHYSFS_exists(dirs[i]) == 0)
		{
			PHYSFS_mkdir(dirs[i]);
		}
	}

	// Generate IO interfaces
	CreateAssimpIO();
	CreateBassIO();
}

void ModuleFileSystem::CreateAssimpIO()
{
	RELEASE(AssimpIO);

	AssimpIO = new aiFileIO;
	AssimpIO->OpenProc = AssimpOpen;
	AssimpIO->CloseProc = AssimpClose;
}

aiFileIO * ModuleFileSystem::GetAssimpIO()
{
	return AssimpIO;
}

// Add a new zip file or folder
bool ModuleFileSystem::AddPath(const char* path_or_zip)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
	{
		ret = true;
	}
	return ret;
}


void ModuleFileSystem::GetExtension(const char * full_path, std::string & extension)
{
	if (full_path)
	{
		std::string path(full_path);
		unsigned int index_dot = path.find_last_of(".");
		if (index_dot < path.length())
		{
			extension = path.substr(index_dot + 1);
		}
	}
}

//Saves the data onto the PHYSFS directory
bool ModuleFileSystem::SaveFile(const void * data, uint data_size, const char * folder, const char * name, const UID &uid, const char * extension)
{
	bool ret = false;

	//Append path
	const uint path_size = 250u;
	char path[path_size];
	sprintf_s(path, path_size, "%s%s_%llu.%s", RESOURCES_MESH_FOLDER, "model", uid, "hinata_mesh");

	//Save write data
	PHYSFS_File * mesh_file = PHYSFS_openWrite(path);
	if (mesh_file != nullptr)
	{
		uint bytes_written = (uint)PHYSFS_write(mesh_file, (const void *)data, 1, data_size);
		//TODO: Download the new version of PHYSFS and use PHYSFS_writeBytes
		if (bytes_written != data_size)
		{
			LOG("Error while writting to file %s: %s", path, PHYSFS_getLastError());
		}
		else
		{
			LOG("Successfully written file %s at %s", path, PHYSFS_getWriteDir());
			ret = true;
		}
	}
	else
	{
		LOG("Errror while opening the file %s: %s", path, PHYSFS_getLastError());
	}

	return ret;
}

const char * ModuleFileSystem::GetBasePath() const
{
	return PHYSFS_getBaseDir();
}

const char * ModuleFileSystem::GetReadPaths() const
{
	static char paths[512];

	paths[0] = '\0';

	char **path;
	for (path = PHYSFS_getSearchPath(); *path != nullptr; path++)
	{
		strcat_s(paths, 512, *path);
		strcat_s(paths, 512, "\n");
	}

	return paths;
}

void ModuleFileSystem::CreateBassIO()
{
	RELEASE(BassIO);

	BassIO = new BASS_FILEPROCS;
	//BassIO->close = BassClose;
	//BassIO->length = BassLength;
	//BassIO->read = BassRead;
	//BassIO->seek = BassSeek;
}

BASS_FILEPROCS * ModuleFileSystem::GetBassIO()
{
	return BassIO;
}

