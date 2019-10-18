#pragma once

#include "Globals.h"
#include "parson/parson.h"

class Application;
struct Event;
#include <string>
class Module
{
private :
	bool enabled;

public:
	const char* name = "";
	Module( bool start_enabled = true, const char* name = ""):name(name),enabled(start_enabled)
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
	virtual void EventRequest(const Event& event) {};
	virtual void DrawConfigurationUi() {};
};