#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ComponentCamera.h"
#include "glew\include\GL\glew.h"
#include "PanelScene.h"
#include <gl\GL.h>
#include "ModuleGui.h"
#include "ModuleInput.h"

#include "ModuleGui.h"
#include "ModuleInput.h"
#include "ModuleImport.h"


ModuleScene::ModuleScene(bool start_enabled) :
	Module(start_enabled)
{
	
	

}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start(JSONFile * config)
{
	//LOG("Loading Intro assets");
	root_gameobject = new GameObject("Root", nullptr);
	bool ret = true;
	camera = new GameObject("Camera", root_gameobject->transform);
	component_camera = camera->CreateComponent<ComponentCamera>();

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));	
	App->import->LoadMesh("Assets/BakerHouse.fbx");
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
	object->OnPostUpdate();
	if (component_camera->IsInfrustrum(object->bounding_box.GetAABB()))
		object->gameobject->OnPostUpdate();
	for (std::vector<ComponentTransform *>::iterator iter = object->children.begin();
		iter != object->children.end();
		++iter)
	{
		GameObjectPostUpdateRecursive((*iter));
	}

	//TODO: Turn into a shortcut
	if (App->input->GetKey(SDL_SCANCODE_DELETE))
	{
		if (ComponentTransform* selected_object = App->gui->GetSelecteTransform())
		{
			DeleteGameObject(selected_object->gameobject);
			App->gui->SetSelectedGameObjec(nullptr);
		}
	}
}

void ModuleScene::DeleteGameObject(GameObject * gameobject)
{
	gameobject->transform->DeleteChildren();
	if (gameobject->transform->parent)
	{
		for (std::vector<ComponentTransform*>::iterator iter = gameobject->transform->parent->children.begin(); iter != gameobject->transform->parent->children.end(); ++iter)
		{
			if ((*iter) == gameobject->transform)
			{
				gameobject->transform->parent->children.erase(iter);
				break;
			}
			
		}
		
	}
	delete gameobject;
}

update_status ModuleScene::PostUpdate()
{
	App->renderer3D->scene_fbo.StartRender(App->gui->panel_scene->current_viewport_size);

	PPlane p(0, 1, 0, 0);
	p.axis = true;
	//p.wire = false;
	p.Render();

	camera->OnPostUpdate();
	GameObjectPostUpdateRecursive(root_gameobject->transform);

	App->renderer3D->scene_fbo.EndRender();

	return UPDATE_CONTINUE;
}