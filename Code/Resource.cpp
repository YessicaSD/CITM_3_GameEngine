#include "Resource.h"
#include "ModuleFileSystem.h"
#include "JSONFile.h"
#include <stdio.h>

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

void Resource::LoadUID(JSONFile * meta_file)
{
	const char * aux_uid = meta_file->LoadText("resourceUID", "0");
	uid = strtoull(aux_uid, nullptr, 10);
}

void Resource::SaveUID(JSONFile * meta_file) const
{
	char buffer[UID_DIGITS];
	sprintf_s(buffer, "%020llu", uid);
	meta_file->SaveText("resourceUID", buffer);
}

void Resource::SaveModifiedDate(JSONFile * meta_file, const char * asset_path)
{
	struct stat file_stat;
	if (stat(asset_path, &file_stat) == 0)
	{
		meta_file->SaveNumber("dateModified", file_stat.st_atime);
	}
}