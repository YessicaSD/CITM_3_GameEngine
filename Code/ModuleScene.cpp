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
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
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
	GameObjectPostUpdateRecursive(root_gameobject->transform);

	PPlane p(0, 1, 0, 0);
	p.axis = true;
	p.wire = false;
	p.Render();

	return UPDATE_CONTINUE;
}

//TODO: Remove this, we shouldn't iterate meshes directly
//We should instead gameobjects
//And if that gameobject has a mesh component, it should draw its mesh
//Maybe if it's on debug, it should draw its normals
//void ModuleScene::DrawFaceNormals()
//{
//	for (std::vector<AssimpScene*>::iterator scene_iter = App->import->array_scene.begin(); scene_iter != App->import->array_scene.end(); ++scene_iter)
//	{
//		for (std::vector<ComponentMesh*>::iterator iter = (*scene_iter)->assimp_meshes.begin(); iter != (*scene_iter)->assimp_meshes.end(); ++iter)
//		{
//			if ((*iter))
//			{
//				(*iter)->DrawNormals();
//			}
//		}
//	}
//}

//void ModuleScene::DrawVertexNormals()
//{
//	for (std::vector<AssimpScene*>::iterator scene_iter = App->import->array_scene.begin(); scene_iter != App->import->array_scene.end(); ++scene_iter)
//	{
//		for (std::vector<ComponentMesh*>::iterator iter = (*scene_iter)->assimp_meshes.begin(); iter != (*scene_iter)->assimp_meshes.end(); ++iter)
//		{
//			if ((*iter))
//			{
//				(*iter)->DrawVertexNormal();
//			}
//		}
//	}
//}