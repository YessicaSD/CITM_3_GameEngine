#ifndef MODULE_FILESYSTEM_H_
#define MODULE_FILESYSTEM_H_
#include "Module.h"
#include <string>
#include <vector>

struct aiFileIO;
#include "Bass/include/bass.h"
//struct BASS_FILEPROCS;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(const char * name);
	bool AddPath(const char * path_or_zip);
	void GetExtension(const char* full_path, std::string& extension);
	void CreatePath(char * path, uint path_size, const char * folder, const UID & uid, const char * extension);
	bool SaveFile(const void * data, uint data_size, char ** path);
	bool LoadFile(const char * path, char ** data);
	const char * GetReadPaths() const;
	const char * GetBasePath() const;
	void GetFilesAndDirs(const char * directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	void SplitFilePath(const char * full_path, std::string * path, std::string * file, std::string * extension) const;
	bool CopyFromOutsideFS(const char * full_path, const char * destination);
	bool FileExists(const char * file) const;

	bool Remove(const char * file);

	void NormalizePath(char * full_path) const;

	void NormalizePath(std::string & full_path) const;

private:
	void CreateAssimpIO();
	aiFileIO * GetAssimpIO();

	void CreateBassIO();
	BASS_FILEPROCS * GetBassIO();


private:
	aiFileIO* AssimpIO = nullptr;
	BASS_FILEPROCS* BassIO = nullptr;
};
#endif // !MODULE_FILESYSTEM_H_
