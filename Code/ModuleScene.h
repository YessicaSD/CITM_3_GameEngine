#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#include "Module.h"
#include "Globals.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

public:
};

#endif // !MODULESCENE_H_

