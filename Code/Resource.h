#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

class Resource
{
public:
	//INFO: Saves the resource with custom format in the Resources folder
	virtual bool SaveResource() = 0;

protected:
	UID uid = 0u;
};

#endif