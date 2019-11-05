#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "Shortcut.h"
#include "PanelProperties.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "MathGeoLib/include/Geometry/AABB.h"

ModuleCamera3D::ModuleCamera3D(const char * name, bool start_enabled) : Module(start_enabled, name)
{
	CalculateViewMatrix();

	x = vec3(1.0f, 0.0f, 0.0f);
	y = vec3(0.0f, 1.0f, 0.0f);
	z = vec3(0.0f, 0.0f, 1.0f);

	position = vec3(0.0f, 0.0f, 5.0f);
	reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start(JSON_Object* config)
{
	LOG("Setting up the camera");
	bool ret = true;

	navigate_forward	= new Shortcut("Move camera forward",	{ SDL_SCANCODE_W });
	navigate_backward	= new Shortcut("Move camera backward",	{ SDL_SCANCODE_S });
	navigate_left		= new Shortcut("Move camera left",		{ SDL_SCANCODE_A });
	navigate_right		= new Shortcut("Move camera right",		{ SDL_SCANCODE_D });
	navigate_up			= new Shortcut("Move camera up",		{ SDL_SCANCODE_Q });
	navigate_down		= new Shortcut("Move camera right",		{ SDL_SCANCODE_E });
	navigate_fast		= new Shortcut("Move camera faster",	{ SDL_SCANCODE_LSHIFT });
	focus_object		= new Shortcut("Focus to object", { SDL_SCANCODE_F });
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (focus_object->Pressed())
	{
		const ComponentTransform* selected_transform = App->gui->panel_properties->GetSelecteTransform();
		if (selected_transform != nullptr)
		{
			FocusToObject((*selected_transform));
		}
	}
	vec3 new_pos(0,0,0);

	float move_speed = camera_move_speed * dt;
	if (navigate_fast->Held())
	{
		move_speed *= 2.f;
	}
	if (navigate_up->Held())
	{
		new_pos.y += move_speed;
	}
	if (navigate_down->Held())
	{
		new_pos.y -= move_speed;
	}
	if (navigate_forward->Held())
	{
		new_pos -=z * move_speed;
	}
	if (navigate_backward->Held())
	{
		new_pos += z * move_speed;
	}
	if (navigate_left->Held())
	{
		new_pos -= x * move_speed;
	}
	if (navigate_right->Held())
	{
		new_pos += x * move_speed;
	}

	int mouse_wheel = App->input->GetMouseWheel();
	if (mouse_wheel != 0)
	{
		new_pos -= z * mouse_wheel * move_speed * 2.f;
	}
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE)==KEY_STATE::KEY_REPEAT)
	{
		new_pos -= x * App->input->GetMouseMotionX() * move_speed * 0.5f;
		new_pos += y * App->input->GetMouseMotionY() * move_speed * 0.5f;
	}

	position += new_pos;
	reference += new_pos;

	// Mouse motion ----------------
	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float rotate_speed = camera_rotate_speed * dt;

		int dx = -App->input->GetMouseMotionX();
		int dy = -App->input->GetMouseMotionY();

		position -= reference;

		if(dx != 0)
		{
			float delta_x = (float)dx * rotate_speed;

			x = rotate(x, delta_x, vec3(0.0f, 1.0f, 0.0f));
			y = rotate(y, delta_x, vec3(0.0f, 1.0f, 0.0f));
			z = rotate(z, delta_x, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float delta_y = (float)dy * rotate_speed;

			y = rotate(y, delta_y, x);
			z = rotate(z, delta_y, x);

			if(y.y < 0.0f)
			{
				z = vec3(0.0f, z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				y = cross(z, x);
			}
		}

		position = reference + z * Length(position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->position = Position;
	this->reference = Reference;

	z = normalize(Position - Reference);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	if(!RotateAroundReference)
	{
		this->reference = this->position;
		this->position += z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	reference = Spot;

	z = normalize(position - reference);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	
}

void ModuleCamera3D::FocusToObject(const ComponentTransform & transform)
{
	ComponentMesh* mesh = transform.gameobject->GetComponent<ComponentMesh>();	
	float3 pos;
	float length;
	if (mesh)
	{
		AABB aux_aabb = mesh->bounding_box;
		pos = mesh->bounding_box.CenterPoint();
		//pos = { (aux_aabb.minPoint + aux_aabb.maxPoint) / 2 };
		length = mesh->bounding_box.Diagonal().Length();
	}
	else
	{
		pos  = transform.GetPosition();
		length = 20;
	}
	if (reference.x != pos.x && reference.y != pos.y && reference.z != pos.z)
		reference = vec3(pos.x, pos.y, pos.z);
	else
		return;

	z = normalize(position - reference);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);
	
	position = vec3(pos.x,pos.y, pos.z) + z * length;
	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	position += Movement;
	reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

bool ModuleCamera3D::SaveConfiguration(JSON_Object * module_obj)
{
	json_object_set_number(module_obj, "move speed", camera_move_speed);
	json_object_set_number(module_obj, "rotate speed", camera_rotate_speed);
	return true;
}

bool ModuleCamera3D::LoadConfiguration(JSON_Object * module_obj)
{
	camera_move_speed = json_object_get_number(module_obj, "move speed");
	camera_rotate_speed = json_object_get_number(module_obj, "rotate speed");
	return true;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(x.x, y.x, z.x, 0.0f, x.y, y.y, z.y, 0.0f, x.z, y.z, z.z, 0.0f, -dot(x, position), -dot(y, position), -dot(z, position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
