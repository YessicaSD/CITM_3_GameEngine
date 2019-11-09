#include "ModuleFileSystem.h"
#include "Module.h"
#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL_filesystem.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

ModuleFileSystem::ModuleFileSystem(const char * name) : Module (true, name)
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
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

// Save a whole buffer to disk
//uint ModuleFileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append) const
//{
//	unsigned int ret = 0;
//
//	bool overwrite = PHYSFS_exists(file) != 0;
//	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);
//
//	if (fs_file != nullptr)
//	{
//		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
//		if (written != size)
//			LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
//		else
//		{
//			if (append == true)
//				LOG("Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
//			//else if(overwrite == true)
//				//LOG("File [%s%s] overwritten with %u bytes", PHYSFS_getWriteDir(), file, size);
//			else if (overwrite == false)
//				LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);
//
//			ret = written;
//		}
//
//		if (PHYSFS_close(fs_file) == 0)
//			LOG("File System error while closing file %s: %s", file, PHYSFS_getLastError());
//	}
//	else
//		LOG("File System error while opening file %s: %s", file, PHYSFS_getLastError());
//
//	return ret;
//}