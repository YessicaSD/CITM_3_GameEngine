#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#include <map>
#include <string>

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

struct ImVec4;
class ComponentCamera;

class ModuleScene : public Module
{	
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start(JSONFile* config) override;
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	//void DrawFaceNormals();
	//void DrawVertexNormals();
	bool CleanUp();
	void ChangeRenderMode(std::string variable);
	bool GetRenderMode(std::string variable);
	void GameObjectPostUpdateRecursive(ComponentTransform * object);

private:
	void DeleteGameObject(GameObject* gameobject);

public:
	//All gameobjects are children of the root gameobject
	GameObject* root_gameobject;
	GameObject* camera;
	ComponentCamera* component_camera;
};

#endif // !MODULESCENE_H_

