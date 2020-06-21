#include "BassIO.h"
#include "../PhysFS/include/physfs.h"
#include "Globals.h"

//void CALLBACK BassClose(void* file)
//{
//	if (PHYSFS_close((PHYSFS_File*)file) == 0)
//	{
//		LOG("File System error while CLOSE via bass: %s", PHYSFS_getLastError());
//	}
//}
//
//QWORD CALLBACK BassLength(void* file)
//{
//	PHYSFS_sint64 ret = PHYSFS_fileLength((PHYSFS_File*)file);
//	if (ret == -1)
//	{
//		LOG("File System error while SIZE via bass: %s", PHYSFS_getLastError());
//	}
//
//	return (QWORD)ret;
//}
//
//DWORD CALLBACK BassRead(void *buffer, DWORD len, void* file)
//{
//	PHYSFS_sint64 ret = PHYSFS_read((PHYSFS_File*)file, buffer, 1, len);
//	if (ret == -1)
//	{
//		LOG("File System error while READ via bass: %s", PHYSFS_getLastError());
//	}
//
//	return (DWORD)ret;
//}
//
//BOOL CALLBACK BassSeek(QWORD offset, void* file)
//{
//	int res = PHYSFS_seek((PHYSFS_File*)file, offset);
//	if (res == 0)
//	{
//		LOG("File System error while SEEK via bass: %s", PHYSFS_getLastError());
//	}
//
//	return (BOOL)res;
//}