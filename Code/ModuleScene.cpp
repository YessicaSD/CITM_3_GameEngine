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
#include "PanelGame.h"
#include <gl\GL.h>

#include "ModuleGui.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "ModuleInput.h"
#include "ModuleImport.h"
#include "ModuleFileSystem.h"
#include "RaycastHit.h"

#include "imGuizmo/ImGuizmo.h"
#include "Event.h"
#include <map>

ModuleScene::ModuleScene(const char * name, bool start_enabled) :
	Module(start_enabled, name)
{
	current_scene_name = "new_scene";
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start(JSONFile * config)
{
	//LOG("Loading Intro assets");
	root_gameobject = new GameObject("Root", nullptr);
	CreateOctree();

	bool ret = true;

	//ResourceModel * resource_model = App->import->ImportModel("Assets/BakerHouse.fbx");
	//App->import->CreateGameObjectFromModel(resource_model, App->scene->root_gameobject->transform);

	//App->import->LoadModelMeta(resource_model, "Assets/BakerHouse.fbx.meta");

	GameObject* object_camera = new GameObject("Main camera", root_gameobject->transform);
	game_camera = object_camera->CreateComponent<ComponentCamera>();

	object_camera = new GameObject("Camera", root_gameobject->transform);
	object_camera->CreateComponent<ComponentCamera>();

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
	if (!App->gui->panel_scene->is_being_used)
	{
		if (App->input->GetKey(SDL_SCANCODE_W))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::TRANSLATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_E))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::ROTATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_R))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::SCALE;
		}
	}
	
	//TODO: Turn into a shortcut
	if (App->input->GetKey(SDL_SCANCODE_DELETE))
	{
		if (ComponentTransform* selected_object = App->gui->GetSelectedTransform())
		{
			App->gui->SetSelectedGameObjec(nullptr);
			DeleteGameObject(selected_object->gameobject);

		}
	}
	return UPDATE_CONTINUE;
}

void ModuleScene::GameObjectPostUpdateRecursive(ComponentTransform * transform)
{
	transform->gameobject->OnPostUpdate();
	for (std::vector<ComponentTransform *>::iterator iter = transform->children.begin();
		iter != transform->children.end();
		++iter)
	{
		GameObjectPostUpdateRecursive((*iter));
	}
}
bool Compare(RaycastHit & a, RaycastHit & b)
{
	return a.distance < b.distance;
}

bool ModuleScene::IntersectRay(LineSegment * ray, RaycastHit& hit)
{
	this->ray = (*ray);

	std::map<float , ComponentTransform*> out_objects;
	octree.CollectIntersections(out_objects, (*ray));
	GetIntersectBoxNonStatics(root_gameobject->transform, ray, out_objects);

	return TestWithTriangles(ray, out_objects, hit);
}

void ModuleScene::RecreateOctree()
{
	CreateOctree();
	LoadStaticObjects();
}

void ModuleScene::CreateOctree()
{
	const float3 center(0.0f, 0.0f, 0.0f);
	const float3 size(OCTREE_X_SIZE, OCTREE_Y_SIZE, OCTREE_Z_SIZE);
	AABB boundary;
	boundary.SetFromCenterAndSize(center, size);
	octree.SetLimits(boundary);
}

void ModuleScene::GetStaticObjects(std::vector<ComponentTransform*>& static_objects)
{
	root_gameobject->transform->GetStaticObjects(static_objects);
}

void ModuleScene::SetMainCamera(ComponentCamera * main_camera)
{
	if (main_camera != nullptr)
	{
		game_camera = main_camera;
		App->renderer3D->game_fbo.camera = game_camera;
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

void ModuleScene::GetIntersectBoxNonStatics(ComponentTransform * object, LineSegment * ray, std::map<float, ComponentTransform*>& out_objects)
{
	if (object->enabled && !object->is_static)
	{
		if (object->Intersect(*ray))
		{
			
			
			float near_hit, far_hit;
			if (ray->Intersects(object->bounding_box.GetOBB(), near_hit, far_hit))
			{
				
				out_objects[near_hit] = object;
			}
		}
		for (auto iter = object->children.begin(); iter != object->children.end(); ++iter)
		{
			GetIntersectBoxNonStatics((*iter), ray, out_objects);
		}
	}
}
bool ModuleScene::TestWithTriangles(LineSegment * ray, std::map<float, ComponentTransform*>& out_objects, RaycastHit& hit_out)
{
	
	for (std::map<float, ComponentTransform*>::iterator iter = out_objects.begin(); iter != out_objects.end(); ++iter)
	{
		ComponentMesh* mesh = (*iter).second->gameobject->GetComponent<ComponentMesh>();
		if (mesh)
		{
			RaycastHit hit;
			if (mesh->Intersect(ray, hit))
			{
				hit_out = hit;
				return true;
			}
		}
	}
	return false;
}

void ModuleScene::LoadStaticObjects()
{
	std::vector<ComponentTransform*> static_objects;
	root_gameobject->transform->GetStaticObjects(static_objects);
	for (std::vector<ComponentTransform*>::iterator iter = static_objects.begin(); iter < static_objects.end(); ++iter)
	{
		octree.Insert((*iter));
	}
}

void ModuleScene::DrawObjects(ComponentCamera * camera)
{
	if (camera->frustum_culling)
	{
		DrawWithFrustrum(camera);
	}
	else
	{
		GameObjectPostUpdateRecursive(root_gameobject->transform);
	}
}

void ModuleScene::DrawWithFrustrum(ComponentCamera * camera)
{
	std::vector<ComponentTransform*> objects;
	octree.CollectIntersections(objects, camera->GetFrustrum());
	root_gameobject->transform->GetIntersectNonStatics(objects, camera->GetFrustrum());
	for (std::vector<ComponentTransform*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->gameobject->OnPostUpdate();
	}
}

void ModuleScene::SaveScene()
{
	current_scene.CreateJSONFile();
	root_gameobject->OnSave(&current_scene);
	current_scene.SaveFile(std::string(ASSETS_FOLDER) + current_scene_name + "." + SCENE_EXTENSION);
	current_scene.CloseFile();
	LOG("Saved scene");
}

void ModuleScene::LoadScene(const char * scene_path)
{
	std::string scene_name;
	App->file_system->SplitFilePath(scene_path, nullptr, &scene_name, nullptr);
	current_scene_name = scene_name;
	current_scene.LoadFile(scene_path);

	for (JSONFile current_gameobject = current_scene.GetSection("GameObject");
		current_gameobject.IsValid();
		current_gameobject  = current_gameobject.GetSection("GameObject"))
	{

	}
}

update_status ModuleScene::PostUpdate()
{
	App->renderer3D->scene_fbo.StartRender(App->gui->panel_scene->current_viewport_size);

	octree.Draw();

	PPlane p(0, 1, 0, 0);
	p.axis = true;
	//p.wire = false;
	p.Render();
	if (App->camera->see_frustrum_culling)
	{
		DrawObjects(game_camera);
	}
	else
	{
		DrawObjects(App->camera->scene_camera);
	}
	

	glLineWidth(10);
	glColor4f(255, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(ray.a.x, ray.a.y, ray.a.z);
	glVertex3f(ray.b.x, ray.b.y, ray.b.z);
	glEnd();
	App->renderer3D->scene_fbo.EndRender();

	App->renderer3D->game_fbo.StartRender(App->gui->panel_game->current_viewport_size);
	if (game_camera != nullptr)
	{
		DrawObjects(game_camera);
	}
	App->renderer3D->game_fbo.EndRender();
	return UPDATE_CONTINUE;
}

void ModuleScene::EventRequest(const Event & event)
{
	if (event.type == Event::UPDATE_OCTREE)
	{
		RecreateOctree();
	}
	else if (event.type == Event::DROPPED_MODEL_TO_SCENE)
	{
		App->import->CreateGameObjectFromModel(event.drop_model_data.model, App->scene->root_gameobject->transform);
	}
	else if (event.type == Event::SAVE_SCENE)
	{
		SaveScene();
	}
	else if (event.type == Event::LOAD_SCENE)
	{
		LoadScene(event.path);
	}
}
