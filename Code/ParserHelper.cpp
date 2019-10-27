#include "ParserHelper.h"

bool SaveFloatArray(JSON_Object * obj, const char * name, const float * arr, const uint size)
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
bool LoadFloatArray(JSON_Object * obj, const char * name, float * values, const uint size)
{
	JSON_Array * arr = json_object_get_array(obj, name);
	for (int i = 0u; i < size; ++i)
	{
		 JSON_Value * json_value = json_array_get_value(arr, i);
		 values[i] = (float)json_value_get_number(json_value);
	}
	return true;
}