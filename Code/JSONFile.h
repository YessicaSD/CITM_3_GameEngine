#ifndef __JSON_FILE_H__
#define __JSON_FILE_H__

#include<string>
#include<vector>
#include "parson/parson.h"
#include "Globals.h"

//18446744073709551615 is the maximum unsigned long long integer, which has 20 characters
//we need +1 for the terminating null character \0
#define UID_DIGITS 20 + 1

typedef unsigned int uint;

class JSONFile
{
public:
	//Manipulate document
	JSONFile();
	JSONFile(JSON_Object * object);
	void CreateJSONFile();
	void CreateJSONFileArray();
	JSONFile GetSection(const char * section_name);
	JSONFile AddSection(const char * section_name);
	JSON_Value* GetValue();

	void LoadFile(const std::string & path);
	void SaveFile(const std::string& path);
	void CloseFile();

	//Read document
	bool LoadBool(const char * variable_name, bool default = false) const;
	double LoadNumber(const char * variable_name, double default = 0.0) const;
	const char * LoadText(const char * variable_name, const char * default = "") const;
	bool LoadFloatArray(const char* name, float* values);
	bool LoadTextArray(const char * name, const char ** arr);
	bool LoadTextVector(const char * name, std::vector<const char*>& values);
	void AddArrayValue(JSON_Value* newValue);
	UID LoadUID() const;

	//Write document
	bool SaveBool(const char * variable_name, bool value);
	bool SaveNumber(const char * variable_name, double value);
	bool SaveText(const char * variable_name, const char * value);
	bool SaveFloatArray(const char* name, const float* arr, const uint size);
	bool SaveTextArray(const char * name, const char ** array, const uint count);
	bool SaveUID(const char* name_variable, const UID & uid);

	bool LoadUIDVector(const char * name, std::vector<UID>& values);
	bool IsValid();

private:
	JSON_Value* value = nullptr;
	JSON_Array* array = nullptr;
	JSON_Object* object = nullptr;

};
#endif