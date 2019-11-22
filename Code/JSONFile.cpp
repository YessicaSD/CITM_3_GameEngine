#include "JSONFile.h"
#include "Globals.h"

JSONFile::JSONFile()
{}

JSONFile::JSONFile(JSON_Object * object) :
	object(object)
{}

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

void JSONFile::CreateJSONFile()
{
	value = json_value_init_object();
	object = json_value_get_object(value);
	if (value == nullptr || object == nullptr)
	{
		LOG("Error creating JSON with path");
	}
}

JSONFile JSONFile::GetSection(const char * section_name)
{
	return JSONFile(json_object_get_object(object, section_name));
}

JSONFile JSONFile::AddSection(const char * section_name)
{
	json_object_set_value(object, section_name, json_value_init_object());
	return JSONFile(json_object_get_object(object, section_name));
}

//Load document
bool JSONFile::LoadBool(const char * variable_name, bool default) const
{
	JSON_Value * variable_value = json_object_get_value(object, variable_name);
	if (variable_value != nullptr && json_value_get_type(variable_value) == JSONBoolean)
	{
		return json_value_get_boolean(variable_value) != 0;
	}
	return default;
}

double JSONFile::LoadNumber(const char * variable_name, double default) const
{
	JSON_Value * variable_value = json_object_get_value(object, variable_name);
	if (variable_value != nullptr && json_value_get_type(variable_value) == JSONNumber)
	{
		return json_value_get_number(variable_value);
	}
	return default;
}

const char * JSONFile::LoadText(const char * variable_name, const char * default) const
{
	JSON_Value * variable_value = json_object_get_value(object, variable_name);
	if (variable_value != nullptr && json_value_get_type(variable_value) == JSONString)
	{
		return json_value_get_string(variable_value);
	}
	return default;
}

bool JSONFile::LoadFloatArray(const char* name, float* values, const uint size)
{
	JSON_Array* arr = json_object_get_array(object, name);
	for (int i = 0u; i < size; ++i)
	{
		JSON_Value* json_value = json_array_get_value(arr, i);
		values[i] = (float)json_value_get_number(json_value);
	}
	return true;
}

//Write document
bool JSONFile::SaveBool(const char * variable_name, bool value)
{
	return json_object_set_boolean(object, variable_name, (value) ? 1 : 0) == JSONSuccess;
}

bool JSONFile::SaveNumber(const char * variable_name, double value)
{
	return json_object_set_number(object, variable_name, value) == JSONSuccess;
}

bool JSONFile::SaveText(const char * variable_name, const char * value)
{
	return json_object_set_string(object, variable_name, value) == JSONSuccess;
}

bool JSONFile::SaveFloatArray(const char* name, const float* arr, const uint size)
{
	JSON_Value* json_value = json_value_init_array();
	JSON_Array* json_array = json_value_get_array(json_value);
	json_object_set_value(object, name, json_value);
	for (uint i = 0u; i < size; ++i)
	{
		json_array_append_number(json_array, arr[i]);
	}
	return true;
}