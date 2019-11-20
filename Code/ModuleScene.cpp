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

#include "RaycastHit.h"
#include "imGuizmo/ImGuizmo.h"
#include "Event.h"
#include <map>
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
	CreateOctree();

	bool ret = true;
	camera = new GameObject("Camera", root_gameobject->transform);
	component_camera = camera->CreateComponent<ComponentCamera>();
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
	if (!App->gui->panel_scene->is_being_used)
	{
		if (App->input->GetKey(SDL_SCANCODE_W))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::TRANSLATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_E))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::SCALE;
		}
		if (App->input->GetKey(SDL_SCANCODE_R))
		{
			App->gui->panel_scene->guizmo_op = ImGuizmo::ROTATE;
		}
	}
	
	//TODO: Turn into a shortcut
	if (App->input->GetKey(SDL_SCANCODE_DELETE))
	{
		if (ComponentTransform* selected_object = App->gui->GetSelecteTransform())
		{
			App->gui->SetSelectedGameObjec(nullptr);
			DeleteGameObject(selected_object->gameobject);

		}
	}
	return UPDATE_CONTINUE;
}

void ModuleScene::GameObjectPostUpdateRecursive(ComponentTransform * object)
{
		object->gameobject->OnPostUpdate();
	for (std::vector<ComponentTransform *>::iterator iter = object->children.begin();
		iter != object->children.end();
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

void ModuleScene::DrawObjects()
{
	if (component_camera->frustum_culling)
	{
		DrawWithFrustrum();
	}
	else
	{
		GameObjectPostUpdateRecursive(root_gameobject->transform);
	}

}

void ModuleScene::DrawWithFrustrum()
{
	std::vector<ComponentTransform*> objects;
	octree.CollectIntersections(objects, component_camera->GetFrustrum());
	root_gameobject->transform->GetIntersectNonStatics(objects, component_camera->GetFrustrum());
	for (std::vector<ComponentTransform*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->gameobject->OnPostUpdate();
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
	DrawObjects();
	


	glLineWidth(10);
	glColor4f(255, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(ray.a.x, ray.a.y, ray.a.z);
	glVertex3f(ray.b.x, ray.b.y, ray.b.z);
	glEnd();
	App->renderer3D->scene_fbo.EndRender();


	App->renderer3D->game_fbo.StartRender(App->gui->panel_scene->current_viewport_size);
	DrawObjects();
	App->renderer3D->game_fbo.EndRender();
	return UPDATE_CONTINUE;
}

void ModuleScene::EventRequest(const Event & event)
{
	if (event.type == Event::UPDATE_OCTREE)
	{
		RecreateOctree();
	}
}
