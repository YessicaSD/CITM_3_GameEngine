#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

ModuleScene::ModuleScene(bool start_enabled) :
	Module(start_enabled)
{
	root_gameobject = new GameObject("Root", nullptr);
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start(JSON_Object* config)
{
	//LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));	

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	DeleteGameObject(root_gameobject);
	return true;
}


// Update: draw background
update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::GameObjectPostUpdateRecursive(ComponentTransform * object)
{
	object->gameobject->OnPostUpdate();
	for(std::vector<ComponentTransform *>::iterator iter = object->children.begin();
		iter != object->children.end();
		++iter)
	{
		GameObjectPostUpdateRecursive((*iter));
	}
}

void ModuleScene::DeleteGameObject(GameObject * gameobject)
{
	if (gameobject->transform->children.size() > 0)
	{
		for (std::vector<ComponentTransform*>::iterator iter = gameobject->transform->children.begin(); iter != gameobject->transform->children.end(); ++iter)
		{
			DeleteGameObject((*iter)->gameobject);
		}
	}
	delete gameobject;
}

update_status ModuleScene::PostUpdate()
{
	App->renderer3D->StartSceneRender();

	PPlane p(0, 1, 0, 0);
	p.axis = true;
	p.wire = false;
	p.Render();

	GameObjectPostUpdateRecursive(root_gameobject->transform);

	App->renderer3D->EndSceneRender();

	return UPDATE_CONTINUE;
}