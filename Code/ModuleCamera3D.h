#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#define CAM_HEIGHT 4.f
#define CAM_OFFSET 12.f
#define CAM_FACTOR_MULTIPLIER 4.f

class ModulePlayer;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, ModulePlayer * playerToFollow, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt) override;
	void Move(float dt);
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	void DebugCamera(float dt);
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
	mat4x4 ViewMatrix, ViewMatrixInverse;
	bool debugCamera = false;
	ModulePlayer * playerToFollow = nullptr;
	SDL_Rect viewport;//The section of the screen this camera is going to be rendering to
};