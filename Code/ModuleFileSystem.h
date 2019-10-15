#ifndef MODULE_FILESYSTEM_H_
#define MODULE_FILESYSTEM_H_
#include "Module.h"
#include <string>
class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem() {};
	void GetExtension(const char* full_path, std::string& extension);
};

#endif // !MODULE_FILESYSTEM_H_
