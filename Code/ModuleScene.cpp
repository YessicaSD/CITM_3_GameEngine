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

//TODO: Remove, only for testing purposes
#include "ResourceModel.h"


ModuleScene::ModuleScene(const char * name, bool start_enabled) :
	Module(start_enabled, name)
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

	//App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(float3(0.f, 0.f, 0.f));

	ResourceModel * resource_model = App->import->ImportModel("Assets/BakerHouse.fbx");
	App->import->CreateGameObjectFromModel(resource_model, App->scene->root_gameobject->transform);

	camera = new GameObject("Camera", root_gameobject->transform);
	component_camera = camera->CreateComponent<ComponentCamera>();
	
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

void ModuleScene::GameObjectPostUpdateRecursive(ComponentTransform * transform)
{
	//	if (component_camera->gameobject->transform == object || !component_camera->frustum_culling || component_camera->IsInFrustum(object->bounding_box.GetOBB()))
	transform->gameobject->OnPostUpdate();
	for (auto iter = transform->children.begin(); iter != transform->children.end(); ++iter)
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

	std::vector<RaycastHit> out_objects;
	GetIntersectBox(root_gameobject->transform, ray, out_objects);

	std::sort(out_objects.begin(), out_objects.end(), Compare);

	return TestWithTriangles(ray, out_objects, hit);

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

void ModuleScene::GetIntersectBox(ComponentTransform * object, LineSegment * ray, std::vector<RaycastHit>& out_objects)
{
	if (object->enabled)
	{
		if (object->Intersect(*ray))
		{
			
			RaycastHit hit(object);
			float near_hit, far_hit;
			if (ray->Intersects(object->bounding_box.GetOBB(), near_hit, far_hit))
			{
				hit.distance = near_hit;
				out_objects.push_back(hit);
			}

			
		}
		for (auto iter = object->children.begin(); iter != object->children.end(); ++iter)
		{
			GetIntersectBox((*iter), ray, out_objects);
		}
	}
}
bool ModuleScene::TestWithTriangles(LineSegment * ray, std::vector<RaycastHit>& out_objects, RaycastHit& hit_out)
{
	
	for (std::vector<RaycastHit>::iterator iter = out_objects.begin(); iter != out_objects.end(); ++iter)
	{
		ComponentMesh* mesh = (*iter).transform->gameobject->GetComponent<ComponentMesh>();
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
update_status ModuleScene::PostUpdate()
{
	App->renderer3D->scene_fbo.StartRender(App->gui->panel_scene->current_viewport_size);

	PPlane p(0, 1, 0, 0);
	p.axis = true;
	//p.wire = false;
	p.Render();

	App->camera->scene_camera->OnPostUpdate();

	GameObjectPostUpdateRecursive(root_gameobject->transform);

	glPushMatrix();
	glMultMatrixf((const GLfloat*)& float4x4::identity.Inverted());
	glLineWidth(10);
	glColor4f(255, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(ray.a.x, ray.a.y, ray.a.z);
	glVertex3f(ray.b.x, ray.b.y, ray.b.z);
	glEnd();
	glPopMatrix();

	App->renderer3D->scene_fbo.EndRender();

	return UPDATE_CONTINUE;
}