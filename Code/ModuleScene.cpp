#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "glew\include\GL\glew.h"
#include "PanelScene.h"
#include <gl\GL.h>
#include "ModuleGui.h"
#include "ModuleInput.h"

#include "ModuleGui.h"
#include "ModuleInput.h"
#include "ModuleImport.h"

//TODO: Remove, only for testing purposes
#include "ModuleFileSystem.h"

ModuleScene::ModuleScene(bool start_enabled) :
	Module(start_enabled)
{
	root_gameobject = new GameObject("Root", nullptr);
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start(JSONFile * config)
{
	//LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	////Save data
	//int file_content = 5;
	//int file_content_2 = 12;
	//uint file_size = sizeof(file_content) + sizeof(file_content_2);
	//char * data = new char[file_size];
	//char * cursor = data;
	//memcpy(cursor, &file_content, sizeof(file_content));
	//cursor += sizeof(file_content);
	//memcpy(cursor, &file_content_2, sizeof(file_content_2));
	//char * path = "TEST_FILE.hinata_test";
	//App->file_system->SaveFile(data, file_size, &path);

	////Load data
	//char * new_data = nullptr;
	//App->file_system->LoadFile(path, &new_data);
	//char * new_cursor = new_data;
	//int new_file_content = 0;
	//memcpy(&new_file_content, cursor, sizeof(file_content));
	//cursor += sizeof(file_content);
	//int new_file_content_2 = 0;
	//memcpy(&new_file_content_2, cursor, sizeof(file_content_2));


	App->import->ImportModel("Assets/BakerHouse.fbx");
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
	p.wire = false;
	p.Render();

	GameObjectPostUpdateRecursive(root_gameobject->transform);

	App->renderer3D->scene_fbo.EndRender();

	return UPDATE_CONTINUE;
}