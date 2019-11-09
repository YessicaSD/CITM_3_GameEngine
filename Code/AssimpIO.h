#ifndef __ASSIMP_IO_H__
#define __ASSIMP_IO_H__

struct aiFileIO;
struct aiFile;
enum aiReturn;
enum aiOrigin;

aiFile* AssimpOpen(aiFileIO* io, const char* name, const char* format);
size_t AssimpWrite(aiFile* file, const char* data, size_t size, size_t chunks);
size_t AssimpRead(aiFile* file, char* data, size_t size, size_t chunks);
size_t AssimpTell(aiFile* file);
size_t AssimpSize(aiFile* file);
void AssimpFlush(aiFile* file);
aiReturn AssimpSeek(aiFile* file, size_t pos, aiOrigin from);
void AssimpClose(aiFileIO* io, aiFile* file);

#endif