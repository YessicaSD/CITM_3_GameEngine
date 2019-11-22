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

void ModuleFileSystem::CreatePath(char * path, uint path_size, const char * folder, const char * name, const UID & uid, const char * extension)
{
	sprintf_s(path, path_size, "%s%s_%llu.%s", folder, name, uid, extension);
}

//Saves the data into the PHYSFS directory
bool ModuleFileSystem::SaveFile(const void * data, uint data_size, char ** path)
{
	bool ret = false;

	PHYSFS_File * file = PHYSFS_openWrite(*path);
	if (file != nullptr)
	{
		uint bytes_written = (uint)PHYSFS_write(file, (const void *)data, 1, data_size);
		//TODO: Download the new version of PHYSFS and use PHYSFS_writeBytes
		if (bytes_written != data_size)
		{
			LOG("Error while writting to file %s: %s", *path, PHYSFS_getLastError());
		}
		else
		{
			LOG("Success writing file %s at %s", *path, PHYSFS_getWriteDir());
			ret = true;
		}

		if (PHYSFS_close(file) == 0)
		{
			LOG("Error while closing file %s: %s", *path, PHYSFS_getLastError());
		}
	}
	else
	{
		LOG("Error while opening the file %s: %s", *path, PHYSFS_getLastError());
	}

	return ret;
}

//Loads a custom file from the PHYSFS directory
bool ModuleFileSystem::LoadFile(const char * path, char ** data)
{
	bool ret = false;
	PHYSFS_File * file = PHYSFS_openRead(path);

	if (file != nullptr)
	{
		PHYSFS_sint64 file_size = PHYSFS_fileLength(file);
		
		if (file_size > 0)
		{
			*data = new char[file_size];
			uint bytes_read = (uint)PHYSFS_read(file, *data, 1, file_size);
			if (bytes_read != file_size)
			{
				LOG("Error while reading file %s: %s", path, PHYSFS_getLastError());
				RELEASE(data);
			}
			else
			{
				LOG("Success reading file %s at %s", path, PHYSFS_getWriteDir());
				ret = true;
			}
		}
		if (PHYSFS_close(file) == 0)
		{
			LOG("Error while closing file %s: %s", path, PHYSFS_getLastError());
		}
	}
	else
	{
		LOG("Error while opening file %s: %s", path, PHYSFS_getLastError());
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

void ModuleFileSystem::GetFilesAndDirs(const char* directory, std::vector<std::string> & file_list, std::vector<std::string> & dir_list) const
{
	char **rc = PHYSFS_enumerateFiles(directory);

	std::string dir(directory);

	for (char **i = rc; *i != nullptr; i++)
	{
		if (PHYSFS_isDirectory((dir + *i).c_str()))
		{
			dir_list.push_back(*i);
		}
		else
		{
			file_list.push_back(*i);
		}
	}

	PHYSFS_freeList(rc);
}

void ModuleFileSystem::SplitFilePath(const char * full_path, std::string * path, std::string * file, std::string * extension) const
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		//NormalizePath(full);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
			{
				*path = full.substr(0, pos_separator + 1);
			}
			else
			{
				path->clear();
			}
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
			{
				*file = full.substr(pos_separator + 1);
			}
			else
			{
				*file = full;
			}
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
			{
				*extension = full.substr(pos_dot + 1);
			}
			else
			{
				extension->clear();
			}
		}
	}
}

bool ModuleFileSystem::CopyFromOutsideFS(const char * full_path, const char * destination)
{
	// Only place we acces non virtual filesystem
	bool ret = false;

	char buf[8192];
	size_t size;

	FILE* source = nullptr;
	fopen_s(&source, full_path, "rb");
	PHYSFS_file* dest = PHYSFS_openWrite(destination);

	if (source && dest)
	{
		while (size = fread_s(buf, 8192, 1, 8192, source))
		{
			PHYSFS_write(dest, buf, 1, size);
		}

		fclose(source);
		PHYSFS_close(dest);
		ret = true;

		LOG("File System copied file [%s] to [%s]", full_path, destination);
	}
	else
	{
		LOG("File System error while copy from [%s] to [%s]", full_path, destination);
	}

	return ret;
}