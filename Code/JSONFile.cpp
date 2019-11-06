#include "JSONFile.h"
#include "Globals.h"

bool JSONFile::SaveFloatArray(JSON_Object* obj, const char* name, const float* arr, const uint size)
{
	JSON_Value* json_value = json_value_init_array();
	JSON_Array* json_array = json_value_get_array(json_value);
	json_object_set_value(obj, name, json_value);
	for (uint i = 0u; i < size; ++i)
	{
		json_array_append_number(json_array, arr[i]);
	}
	return true;
}


bool JSONFile::LoadFloatArray(JSON_Object* obj, const char* name, float* values, const uint size)
{
	JSON_Array* arr = json_object_get_array(obj, name);
	for (int i = 0u; i < size; ++i)
	{
		JSON_Value* json_value = json_array_get_value(arr, i);
		values[i] = (float)json_value_get_number(json_value);
	}
	return true;
}

void JSONFile::LoadFile(const std::string & path)
{
	value = json_parse_file(path.c_str());
	object = json_object(value);
}

void JSONFile::SaveFile(const std::string & path)
{
	//TODO: Look if it's open
	json_serialize_to_file_pretty(value, path.data());
}

void JSONFile::CloseFile()
{
	json_value_free(value);
	object = nullptr;
	value = nullptr;
}

void JSONFile::CreateNewFile(const std::string & path)
{
	value = json_value_init_object();
	object = json_value_get_object(value);
	if (value == nullptr || object == nullptr)
	{
		LOG("Error creating JSON with path %s", path.data());
	}
}

void JSONFile::AddSection(std::string section_name)
{
	json_object_set_value(value, );
}