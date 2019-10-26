#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Shortcut;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start(JSON_Object* config);
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:
	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

private:
	Shortcut * navigate_forward = nullptr;
	Shortcut * navigate_backward = nullptr;
	Shortcut * navigate_left = nullptr;
	Shortcut * navigate_right = nullptr;
	Shortcut * navigate_up = nullptr;
	Shortcut * navigate_down = nullptr;
	Shortcut * navigate_fast = nullptr;
	Shortcut * focus_object = nullptr;


	mat4x4 ViewMatrix, ViewMatrixInverse;
};