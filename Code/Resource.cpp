#include "Resource.h"

UID Resource::GetUID() const
{
	return uid;
}

void GetFileData(void ** info, void ** data_cursor, size_t bytes)
{
	memcpy(*data_cursor, *info, bytes);
	data_cursor += bytes;
}

void SetFileData(void * data, void * cursor, size_t bytes)
{

}