#pragma once

#include "Globals.h"
#include "parson/parson.h"

class Application;

class Module
{
private :
	bool enabled;

public:
	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool Save(JSON_Object * config)
	{
		return true;
	}
};