#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);


	Position = vec3(0.0f, 0.0f, 0.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	viewport.x = 0.f;
	viewport.y = 0.f;
	viewport.w = SCREEN_WIDTH;
	viewport.h = SCREEN_HEIGHT;


}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	vec3 fVec = vec3(1,0,0);
	Position = vec3(0, 0, 0);
	LookAt(Position);
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

update_status ModuleCamera3D::PreUpdate(float dt) {
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		debugCamera = !debugCamera;
	}
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (debugCamera) {
		DebugCamera(dt);
	}
	else {
		Move(dt);
	}

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Move(float dt)
{
	//TODO: MAKE THE CAMERA MOVE

	//MAKE THE CAMERA MOVE, ROTATE, WITH THE ALT AS DEFAULT

	//ALSO MAKE THAT YOU CAN CHANGE THE WAY TO DO IT IF THE USER WANTS TO.
}

update_status ModuleCamera3D::PostUpdate(float dt) {
	//Get the camera perspective
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(GetViewMatrix());

	//Set the camera viewport
	glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
	//glScissor(viewport.x, viewport.y, viewport.w, viewport.h);

	//Draw all objects from that camera's perspective and viewport
	App->DrawAll();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::DebugCamera(float dt)
{
	vec3 newPos(0, 0, 0);

	int x_motion = -App->input->GetMouseXMotion(0);
	int y_motion = -App->input->GetMouseYMotion(0);

	//Zoom
	if (App->input->GetScrollWheelState(0) == SCROLL_FORWARD) {
		newPos -= Z * 300.0f * dt;
	}
	else if (App->input->GetScrollWheelState(0) == SCROLL_BACKWARD) {
		newPos += Z * 300.0f * dt;
	}

	//Pan
	if (App->input->GetMouseButton(0, MOUSE_RIGHT)) {
		newPos += X * 30.0f * x_motion * dt;
		newPos += Y * 30.0f * -y_motion * dt;
	}
	Position += newPos;
	Reference += newPos;

	// Rotate
	if(App->input->GetMouseButton(0, MOUSE_LEFT) == KEY_REPEAT)
	{
		float Sensitivity = 0.25f;
		Position -= Reference;
		if(x_motion != 0)
		{
			float DeltaX = (float)x_motion * Sensitivity;
			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}
		if(y_motion != 0)
		{
			float DeltaY = (float)y_motion * Sensitivity;
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