#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Shortcut;
class ComponentTransform;
class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(const char * name, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start(JSONFile * config) override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void FocusToObject(const ComponentTransform& transform);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void DrawConfigurationUi() override;
	bool SaveConfiguration(JSONFile* module_file) override;
	bool LoadConfiguration(JSONFile* module_file) override;

private:
	void CalculateViewMatrix();

public:
	
	vec3 x, y, z, position, reference;

private:
	float camera_move_speed = 150.f;
	float camera_rotate_speed = 20.f;

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