#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

#define INVALID_RESOURCE_UID 0

class Resource
{
public:
	//INFO: Saves the resource with custom format in the Resources folder
	virtual bool SaveFileData() = 0;
	virtual bool LoadFileData() = 0;
	virtual bool ReleaseData() = 0;

	bool StartUsingResource();
	bool StopUsingResource();

	UID GetUID() const;

protected:
	//Helpers for LoadFileData() and SaveFileData()
	void CopyToFile(void* info, char* data_cursor, size_t bytes);
	void CopyToMemory(void* info, char* data_cursor, size_t bytes);



protected:
	UID uid = 0u;
	uint reference_count = 0u;//How many GameObjects are currently using this resource
};

#endif