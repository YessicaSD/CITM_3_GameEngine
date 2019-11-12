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
	bool SaveFile(const void * data, uint data_size, const char * folder, const char * name, const UID & uid, const char * extension);
	const char * GetReadPaths() const;
	const char * GetBasePath() const;
	void CreateBassIO();
	BASS_FILEPROCS * GetBassIO();

private:
	aiFileIO* AssimpIO = nullptr;
	BASS_FILEPROCS* BassIO = nullptr;
};
#endif // !MODULE_FILESYSTEM_H_
