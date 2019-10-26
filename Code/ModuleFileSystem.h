#ifndef MODULE_FILESYSTEM_H_
#define MODULE_FILESYSTEM_H_
#include "Module.h"
#include <string>
class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(const char * name);
	void GetExtension(const char* full_path, std::string& extension);

	//Parser helpers
	bool AddFloatArray(JSON_Object * obj, const char * name, const float * arr, const uint size);
};

#endif // !MODULE_FILESYSTEM_H_
