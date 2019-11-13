#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

class Resource
{
public:
	//INFO: Saves the resource with custom format in the Resources folder
	virtual bool SaveFileData() = 0;
	virtual bool LoadFileData(char * data) = 0;
	UID GetUID() const;

	void CopyToFile(void* info, char* data_cursor, size_t bytes);
	void CopyToMemory(void* info, char* data_cursor, size_t bytes);

protected:
	UID uid = 0u;
};

#endif