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

bool ModuleFileSystem::SetFloatArray(JSON_Object * obj, const char * name, const float * arr, const uint size)
{
	JSON_Value * json_value = json_value_init_array();
	JSON_Array * json_array = json_value_get_array(json_value);
	json_object_set_value(obj, name, json_value);
	for (uint i = 0u; i < size; ++i)
	{
		json_array_append_number(json_array, arr[i]);
	}
	return true;
}