#ifndef __JSON_FILE_H__
#define __JSON_FILE_H__

#include<string>
#include "parson/parson.h"

typedef unsigned int uint;

class JSONFile
{
public:
	//Manipulate document
	JSONFile();
	JSONFile(JSON_Object * object);
	void CreateJSONFile(const std::string& path);
	JSONFile AddSection(const char * section_name);
	void LoadFile(const std::string & path);
	void SaveFile(const std::string& path);
	void CloseFile();

	//Read document
	bool LoadBool(const char * variable_name, bool default = false) const;
	double LoadNumber(const char * variable_name, double default = 0.0) const;
	const char * LoadText(const char * variable_name, const char * default = "") const;
	bool LoadFloatArray(const char* name, float* values, const uint size);

	//Write document
	bool SaveBool(const char * variable_name, bool value);
	bool SaveNumber(const char * variable_name, double value);
	bool SaveText(const char * variable_name, const char * value);
	bool SaveFloatArray(const char* name, const float* arr, const uint size);


private:
	JSON_Value* value = nullptr;
	JSON_Object* object = nullptr;
};
#endif