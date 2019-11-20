#include "Resource.h"
#include "ModuleFileSystem.h"

UID Resource::GetUID() const
{
	return uid;
}

void Resource::CopyToFile(void * info, char ** data_cursor, size_t bytes)
{
	memcpy(*data_cursor, info, bytes);
	*data_cursor += bytes;
}

void Resource::CopyToMemory(void* info, char ** data_cursor, size_t bytes)
{
	memcpy(info, *data_cursor, bytes);
	*data_cursor += bytes;
}

//INFO: Keeps the resource but deletes all its data
bool Resource::ReleaseData()
{
	bool ret = false;

	return ret;
}

//INFO: Called each time a GameObject needs to use this resource
//INFO: Only load resource once
bool Resource::StartUsingResource()
{
	if (reference_count > 0u)
	{
		++reference_count;
	}
	else
	{
		if (LoadFileData())
		{
			++reference_count;
		}
	}
	return reference_count > 0u;
}

uint Resource::GetReferenceCount() const
{
	return reference_count;
}

//INFO: Called each time a GameObject stops using this resource
//INFO: Unload resource when no GameObject references it
bool Resource::StopUsingResource()
{
	bool ret = true;
	--reference_count;
	if (reference_count == 0u)
	{
		ret = ReleaseData();
	}
	return ret;
}
