#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#include "Module.h"
#include "Globals.h"

class ModuleScene : public Module
{
private:
	uint my_id;
	uint num_vertices = 3;
	uint my_indices = 0;

	//float vertices[4*3];
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	bool CleanUp();

public:
};

#endif // !MODULESCENE_H_

