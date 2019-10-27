#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "Shortcut.h"
#include "PanelProperties.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "MathGeoLib/include/Geometry/AABB.h"
ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
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
	vec3 newPos(0,0,0);
	float speed = 3.0f * dt;

	if(navigate_fast->Held())
		speed = 8.0f * dt;

	if(navigate_up->Held()) 
		newPos.y += speed;

	if(navigate_down->Held())
		newPos.y -= speed;

	//This is not continous
	//This is just for when the shortcut is pressed
	if(navigate_forward->Held()) 
		newPos -=Z * speed;

	
	int mouse_wheel = App->input->GetMouseWheel();
	if (mouse_wheel != 0)
	{
		newPos -=Z* mouse_wheel * speed*2;
	}
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE)==KEY_STATE::KEY_REPEAT)
	{
		newPos -= X* App->input->GetMouseMotionX()*speed*0.5f;
		newPos += Y*App->input->GetMouseMotionY()*speed*0.5f;
	}
	if(navigate_backward->Held())
		newPos += Z * speed;

	if(navigate_left->Held())
		newPos -= X * speed;

	if(navigate_right->Held())
		newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseMotionX();
		int dy = -App->input->GetMouseMotionY();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * Length(Position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	
}

void ModuleCamera3D::FocusToObject(const ComponentTransform & transform)
{
	ComponentMesh* mesh = transform.gameobject->GetComponent<ComponentMesh>();
	float3 pos;
	float length;
	if (mesh)
	{
		AABB aux_aabb = mesh->boinding_box;
		pos = mesh->boinding_box.CenterPoint();
		//pos = { (aux_aabb.minPoint + aux_aabb.maxPoint) / 2 };
		length = mesh->boinding_box.Diagonal().Length();
	}
	else
	{
		pos  = transform.position;
		length = 20;
	}
	Reference = vec3(pos.x, pos.y, pos.z);
	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	Position = vec3(pos.x,pos.y, pos.z) + Z * length;
	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
