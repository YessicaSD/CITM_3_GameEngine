#pragma once

#include "Globals.h"
#include "parson/parson.h"

class Application;
struct Event;
struct JSON_OBJECT;
class JSONFile;

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

	virtual bool Init(JSONFile * module_file) 
	{
		return true; 
	}

	virtual bool Start(JSONFile * module_file)
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
	
	virtual bool LoadConfiguration(JSONFile * module_file)
	{
		return true;
	}
	
	virtual bool SaveConfiguration(JSONFile * module_file)
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