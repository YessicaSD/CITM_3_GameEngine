#ifndef __JSON_FILE_H__
#define __JSON_FILE_H__

#include<string>
#include "parson/parson.h"

typedef unsigned int uint;

class JSONFile
{
public:
	void CloseFile();
	void CreateNewFile(const std::string& path);
	bool SaveFloatArray(JSON_Object* obj, const char* name, const float* arr, const uint size);
	bool LoadFloatArray(JSON_Object* obj, const char* name, float* values, const uint size);

	void LoadFile(const std::string & path);

	void SaveFile(const std::string& path);

private:
	JSON_Value* value = nullptr;
	JSON_Object* object = nullptr;
};
#endif