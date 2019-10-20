#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#include <map>
#include <string>

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

struct ImVec4;

class ModuleScene : public Module
{	
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

private:
	void GameObjectPostUpdateRecursive(ComponentTransform * object);

public:
	//All gameobjects are children of the root gameobject
	GameObject root_gameobject;
	std::map<std::string, bool> view_mode;
};

#endif // !MODULESCENE_H_

