#pragma once

#include "Globals.h"
#include "parson/parson.h"

class Application;
struct Event;
struct JSON_OBJECT;

#include <string>
class Module
{
private :
	bool enabled = true;

public:
	const char* name = "";
	Module( bool start_enabled = true, const char* name = ""): name(name),enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_Object* config) 
	{
		return true; 
	}

	virtual bool Start(JSON_Object* config)
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
	
	virtual bool LoadConfiguration(JSON_Object * config)
	{
		return true;
	}
	
	virtual bool SaveConfiguration(JSON_Object * config)
	{
		return true;
	}
	
	virtual void EventRequest(const Event& event) {};
	
	virtual void DrawConfigurationUi() {};
	
	bool IsActive() {
		return enabled;
	}
	
	void Disable()
	{
		enabled = false;
		CleanUp();
	}
};