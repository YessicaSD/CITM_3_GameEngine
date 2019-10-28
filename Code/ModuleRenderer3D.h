#ifndef __MODULE_RENDER_3D_H__
#define __MODULE_RENDER_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "RenderTexture.h"

#define MAX_LIGHTS 8

struct SDL_Window;

class ModuleRenderer3D : public Module
{
private:
	bool depth_test = false;
	bool cull_face = false;
	bool lighting = false;
	bool color_material = false;
	bool texture_2d = false;
public:
	ModuleRenderer3D(const char* name, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate() override;
	update_status PostUpdate() override;
	bool CleanUp();

	void OnResize(int width, int height);
	
	void DrawConfigurationUi();
public:
	Light lights[MAX_LIGHTS];
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, projection_matrix;

	RenderTexture scene_fbo;

	float camera_near = 0.125f;
	float camera_far = 512.f;
	float fov = 60.f;

	friend class PanelConfiguration;
};

#endif