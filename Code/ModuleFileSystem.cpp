#include "ModuleFileSystem.h"
#include "Module.h"

ModuleFileSystem::ModuleFileSystem(const char * name) : Module (true, name)
{}

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