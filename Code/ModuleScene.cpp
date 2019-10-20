#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

ModuleScene::ModuleScene(bool start_enabled) :
	Module(start_enabled),
	root_gameobject("Root", nullptr)
{
	view_mode["default"] = true;
	view_mode["wireframe"] = false;
	view_mode["vertex"] = false;
	view_mode["vertices_normals"] = false;
	view_mode["face_normals"] = false;
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

update_status ModuleScene::PostUpdate()
{
	GameObjectPostUpdateRecursive(&root_gameobject.transform);

	//if (view_mode["default"])
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glColor3f(1, 1, 1);
	//	Draw();
	//}

	//if (view_mode["wireframe"])
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	glColor3f(0, 0, 1);
	//	Draw();
	//}

	//if (view_mode["vertex"])
	//{
	//	glColor3f(1, 0, 0);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//	glPointSize(5);
	//	Draw();
	//}
	//if (view_mode["vertices_normals"])
	//{
	//	glColor3f(0, 1, 0);
	//	glLineWidth(5);
	//	DrawVertexNormals();
	//}
	//if (view_mode["face_normals"])
	//{
	//	glColor3f(0, 1, 0);
	//	glLineWidth(5);
	//	DrawFaceNormals();
	//}

	//glColor3f(1, 1, 1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

void ModuleScene::ChangeRenderMode(std::string variable)
{
	std::map<std::string, bool>::iterator iter = view_mode.find(variable);
	if (iter != view_mode.end())
	{
		(*iter).second = !(*iter).second;
	}
}

bool ModuleScene::GetRenderMode(std::string variable)
{
	std::map<std::string, bool>::iterator iter = view_mode.find(variable);
	if (iter != view_mode.end())
	{
		return (*iter).second;
	}
	return false;
}