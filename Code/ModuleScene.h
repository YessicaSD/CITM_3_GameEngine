#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#include <map>
#include <string>

#include "Module.h"
#include "Globals.h"
#include "Cube.h"
#include "GameObject.h"

typedef struct par_shapes_mesh_s par_shapes_mesh;

class ModuleScene : public Module
{
private:
	uint my_id;
	uint num_vertices = 3;
	uint my_indices = 0;
	Cube* cube[2];


	std::map<std::string, bool> view_mode;
	
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	//void DrawFaceNormals();
	//void DrawVertexNormals();
	bool CleanUp();
	void ChangeRenderMode(std::string variable);
	bool GetRenderMode(std::string variable);

	void CreateMenu();

	//All gameobjects are children of the root gameobject
	GameObject root_gameobject;
};

#endif // !MODULESCENE_H_

