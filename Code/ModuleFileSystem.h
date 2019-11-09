#ifndef MODULE_FILESYSTEM_H_
#define MODULE_FILESYSTEM_H_
#include "Module.h"
#include <string>
class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(const char * name);
	void GetExtension(const char* full_path, std::string& extension);
	//uint Save(const char * file, const void * buffer, unsigned int size, bool append);
};
#endif // !MODULE_FILESYSTEM_H_
