#ifndef MODULE_FILESYSTEM_H_
#define MODULE_FILESYSTEM_H_
#include "Module.h"
#include <string>

struct aiFileIO;
#include "Bass/include/bass.h"
//struct BASS_FILEPROCS;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(const char * name);
	void CreateAssimpIO();
	aiFileIO * GetAssimpIO();
	bool AddPath(const char * path_or_zip);
	void GetExtension(const char* full_path, std::string& extension);
	const char * GetBasePath() const;
	const char * GetReadPaths() const;
	void CreateBassIO();
	BASS_FILEPROCS * GetBassIO();
	//uint Save(const char * file, const void * buffer, unsigned int size, bool append);

private:
	aiFileIO* AssimpIO = nullptr;
	BASS_FILEPROCS* BassIO = nullptr;
};
#endif // !MODULE_FILESYSTEM_H_
