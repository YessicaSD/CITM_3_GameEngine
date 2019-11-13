#include "Globals.h"
#include "Application.h"

#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"

#include "Shortcut.h"
#include "PanelProperties.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"

#include "glew/include/GL/glew.h"
#include "PanelScene.h"
ModuleCamera3D::ModuleCamera3D(const char *name, bool start_enabled) : Module(start_enabled, name)
{
	reference = { 0.0f, 0.0f, 0.0f };
}

ModuleCamera3D::~ModuleCamera3D()
{
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start(JSONFile* config)
{
	LOG("Setting up the camera");
	bool ret = true;

	scene_camera = new ComponentCamera(nullptr);
	scene_camera->SetPos(float3(0,2,10));
	scene_camera->LookAt(reference);

	navigate_forward = new Shortcut("Move camera forward", {SDL_SCANCODE_W});
	navigate_backward = new Shortcut("Move camera backward", {SDL_SCANCODE_S});
	navigate_left = new Shortcut("Move camera left", {SDL_SCANCODE_A});
	navigate_right = new Shortcut("Move camera right", {SDL_SCANCODE_D});
	navigate_up = new Shortcut("Move camera up", {SDL_SCANCODE_Q});
	navigate_down = new Shortcut("Move camera right", {SDL_SCANCODE_E});
	navigate_fast = new Shortcut("Move camera faster", {SDL_SCANCODE_LSHIFT});
	focus_object = new Shortcut("Focus to object", {SDL_SCANCODE_F});

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

bool Compare(RaycastHit & a, RaycastHit & b)
{
	return a.distance < b.distance;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	//if (focus_object->Pressed())
	//{
	//	const ComponentTransform *selected_transform = App->gui->panel_properties->GetSelecteTransform();
	//	if (selected_transform != nullptr)
	//	{
	//		FocusToObject((ComponentTransform)(*selected_transform));
	//	}
	//}

	float3 new_pos(0, 0, 0);
	float move_speed = camera_move_speed * dt;

	if (navigate_fast->Held())
	{
		move_speed *= 2.f;
	}

	if (navigate_up->Held())
	{
		new_pos += move_speed * scene_camera->frustum.up;
	}
	if (navigate_down->Held())
	{
		new_pos -= move_speed * scene_camera->frustum.up;
	}

	if (navigate_forward->Held())
	{
		new_pos += scene_camera->frustum.front * move_speed;
	}
	if (navigate_backward->Held())
	{
		new_pos -= scene_camera->frustum.front * move_speed;
	}

	if (navigate_left->Held())
	{
		new_pos -= scene_camera->frustum.WorldRight() * move_speed;
	}
	if (navigate_right->Held())
	{
		new_pos += scene_camera->frustum.WorldRight() * move_speed;
	}

	
	int mouse_wheel = App->input->GetMouseWheel();
	if (mouse_wheel != 0)
	{
		new_pos += scene_camera->frustum.front * mouse_wheel * move_speed * 2.f;
	}
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_STATE::KEY_REPEAT)
	{
		new_pos -= scene_camera->frustum.WorldRight() * App->input->GetMouseMotionX() * move_speed * 0.5f;
		new_pos += scene_camera->frustum.up * App->input->GetMouseMotionY() * move_speed * 0.5f;
	}

	scene_camera->SetPos(scene_camera->frustum.pos + new_pos);
	reference += new_pos;

	// Mouse motion ----------------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		RotateCamera(dt);
	}
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		
		float width  = App->gui->panel_scene->width;
		float height  = App->gui->panel_scene->height;

		float x_pos = -(1.0f - (float(App->gui->panel_scene->cursor.x) * 2.0f) / width);
		float y_pos = 1.0f - (float(App->gui->panel_scene->cursor.y) * 2.0f) / height;

		LOG("X = %.2f, Y=%.2f", App->gui->panel_scene->cursor.x, App->gui->panel_scene->cursor.y);
		picking = scene_camera->frustum.UnProjectLineSegment(x_pos,y_pos);
		std::vector<RaycastHit> hit_object;
		App->scene->IntersectRay(&picking, hit_object);
		std::sort(hit_object.begin(), hit_object.end(),Compare);
		if (hit_object.size()>0)
		{
			App->gui->SetSelectedGameObjec(hit_object[0].transform);
		}

	}
	return UPDATE_CONTINUE;
}

update_status ModuleCamera3D::PostUpdate()
{

	return update_status::UPDATE_CONTINUE;
}

void ModuleCamera3D::RotateCamera(float dt)
{
	float rotate_speed = camera_rotate_speed * dt;
	float dx = (float)-App->input->GetMouseMotionX() * rotate_speed;
	float dy = (float)-App->input->GetMouseMotionY() * rotate_speed;

	if (dx != 0 || dy != 0)
	{
		float3 vector = scene_camera->frustum.pos - reference;
		Quat quat_y(scene_camera->frustum.up, dx  * DEGTORAD);
		Quat quat_x(scene_camera->frustum.WorldRight(), dy  * DEGTORAD);
		Quat result_rotation = quat_y * quat_x;
		vector = result_rotation.Transform(vector);
		scene_camera->frustum.pos = vector + reference;
		LookAt(reference);
	}
}

//// -----------------------------------------------------------------
//void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
//{
//	this->position = Position;
//	this->reference = Reference;
//
//	z = (Position - Reference).Normalized();
//	x = (z.Cross(float3(0.0f, 1.0f, 0.0f))).Normalized();
//	y = z.Cross(x);
//
//	if (!RotateAroundReference)
//	{
//		this->reference = this->position;
//		this->position += z * 0.05f;
//	}
//
//	CalculateViewMatrix();
//}

////// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3 &spot)
{
	reference = spot;
	scene_camera->LookAt(spot);
}

//void ModuleCamera3D::FocusToObject( ComponentTransform &transform)
//{
//	ComponentMesh *mesh = transform.gameobject->GetComponent<ComponentMesh>();
//	float3 pos;
//	float length;
//	if (mesh)
//	{
//		AABB aux_aabb = transform.GetAABB();
//		pos = aux_aabb.CenterPoint();
//		length = aux_aabb.Diagonal().Length();
//	}
//	else
//	{
//		pos = transform.GetPosition();
//		length = 20;
//	}
//	if (reference.x != pos.x && reference.y != pos.y && reference.z != pos.z)
//		reference = float3(pos.x, pos.y, pos.z);
//	else
//		return;
//
//	z = (position - reference);
//	z.Normalized();
//	x = { 0.0f, 1.0f, 0.0f };
//	x = x.Cross(z);
//	x.Normalized();
//	y = z.Cross(x);
//
//	position = float3(pos.x, pos.y, pos.z) + z * length;
//	CalculateViewMatrix();
//}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	scene_camera->frustum.pos += Movement;
	reference += Movement;

}

// -----------------------------------------------------------------
float *ModuleCamera3D::GetViewMatrix()
{
	return (float*)&scene_camera->GetViewMatrix();
}

bool ModuleCamera3D::SaveConfiguration(JSONFile * module_file)
{
	module_file->SaveNumber("move speed", camera_move_speed);
	module_file->SaveNumber("rotate speed", camera_rotate_speed);
	return true;
}

bool ModuleCamera3D::LoadConfiguration(JSONFile * module_file)
{
	camera_move_speed = module_file->LoadNumber("move speed");
	camera_rotate_speed = module_file->LoadNumber("rotate speed");
	return true;
}

float3 ModuleCamera3D::GetPos()
{
	return scene_camera->frustum.pos;
}



