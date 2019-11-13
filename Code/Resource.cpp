#include "Resource.h"

UID Resource::GetUID() const
{
	return uid;
}

void Resource::CopyToFile(void * info, char * data_cursor, size_t bytes)
{
	memcpy(data_cursor, info, bytes);
	data_cursor += bytes;
}

void Resource::CopyToMemory(void* info, char* data_cursor, size_t bytes)
{
	memcpy(info, data_cursor, bytes);
	data_cursor += bytes;
}