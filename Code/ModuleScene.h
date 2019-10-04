#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#include "Module.h"
#include "Globals.h"
#include "Cube.h"

typedef struct par_shapes_mesh_s par_shapes_mesh;

class ModuleScene : public Module
{
private:
	uint my_id;
	uint num_vertices = 3;
	uint my_indices = 0;
	Cube* cube[2];
	par_shapes_mesh* sphereInfo = nullptr;
	uint sphere_v_id;
	uint sphere_indice_id;
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

