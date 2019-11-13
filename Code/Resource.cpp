#include "Resource.h"
#include "ModuleFileSystem.h"

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

void Resource::StartUsingResource()
{
	if (reference_count > 0u)
	{
		reference_count++;
	}
	else
	{
		//Ony load the resource once
		App->file_system->LoadFile();
		//DDS don't need to use this
		//LoadFileData should call LoadFile
		LoadFileData();
	}
}

void Resource::StopUsingResource()
{
	if ()
	{

	}
}
