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

void JSONFile::CreateJSONFileArray()
{
	value = json_value_init_array();
	array = json_value_get_array(value);
	if (value == nullptr || object == nullptr)
	{
		LOG("Error creating JSON Array");
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

JSON_Value * JSONFile::GetValue()
{
	return value;
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

bool JSONFile::LoadFloatArray(const char* name, float* values)
{
	JSON_Array* arr = json_object_get_array(object, name);
	for (int i = 0u; i < json_array_get_count(arr); ++i)
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

bool JSONFile::SaveFloatArray(const char* name, const float* arr, const uint count)
{
	bool ret = true;
	JSON_Value* json_value = json_value_init_array();
	JSON_Array* json_array = json_value_get_array(json_value);
	json_object_set_value(object, name, json_value);
	for (uint i = 0u; i < count && ret; ++i)
	{
		ret = json_array_append_number(json_array, arr[i]) == JSONSuccess;
	}
	return ret;
}

bool JSONFile::LoadTextArray(const char* name, const char ** values)
{
	JSON_Array* arr = json_object_get_array(object, name);
	for (int i = 0u; i < json_array_get_count(arr); ++i)
	{
		JSON_Value* json_value = json_array_get_value(arr, i);
		values[i] = json_value_get_string(json_value);
	}
	return true;
}

bool JSONFile::LoadTextVector(const char* name, std::vector<const char *> &values)
{
	JSON_Array* arr = json_object_get_array(object, name);
	for (int i = 0u; i < json_array_get_count(arr); ++i)
	{
		JSON_Value* json_value = json_array_get_value(arr, i);
		values.push_back(json_value_get_string(json_value));
	}
	return true;
}

void JSONFile::AddArrayValue(JSON_Value * newValue)
{
	if (newValue != nullptr)
	{
		json_array_append_value(array, newValue);
	}
}

bool JSONFile::SaveTextArray(const char * name, const char ** arr, const uint count)
{
	bool ret = true;
	JSON_Value* json_value = json_value_init_array();
	JSON_Array* json_array = json_value_get_array(json_value);
	json_object_set_value(object, name, json_value);
	for (uint i = 0u; i < count && ret; ++i)
	{
		ret = json_array_append_string(json_array, arr[i]) == JSONSuccess;
	}
	return ret;
}

UID JSONFile::LoadUID() const
{
	const char * aux_uid = LoadText("resourceUID", "0");
	return strtoull(aux_uid, nullptr, 10);
}

bool JSONFile::SaveUID(const char* name_variable, const UID & uid)
{
	char buffer[UID_DIGITS];
	sprintf_s(buffer, "%020llu", uid);
	return SaveText(name_variable, buffer);
}

bool JSONFile::LoadUIDVector(const char * name, std::vector<UID> & values)
{
	JSON_Array* arr = json_object_get_array(object, name);
	for (int i = 0u; i < json_array_get_count(arr); ++i)
	{
		JSON_Value* json_value = json_array_get_value(arr, i);
		values.push_back(strtoull(json_value_get_string(json_value), nullptr, 10));
	}
	return true;
}

bool JSONFile::IsValid()
{
	return object!=nullptr;
}
