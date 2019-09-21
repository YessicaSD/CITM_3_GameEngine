#ifndef MODULESCENE01_H_
#define MODULESCENE01_H_

#include "Module.h"
#include "Globals.h"

class ModuleScene01 : public Module
{
public:
	ModuleScene01(Application* app, bool start_enabled = true);
	~ModuleScene01();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
};

#endif // !MODULESCENE01_H_

