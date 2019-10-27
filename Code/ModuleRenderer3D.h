#ifndef __MODULE_WINDOW_H__
#define __MODULE_WINDOW_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

struct SDL_Window;
struct ImVec2;

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

	bool Init(JSON_Object* config);
	void GenSceneFramebuffer();
	update_status PreUpdate() override;
	void PrepareTextureBuffer(ImVec2 &size);
	void PrepareDepthBuffer(ImVec2 &size);
	void PrepareCamera(ImVec2 &size);
	update_status PostUpdate() override;
	bool CleanUp();

	bool SaveConfiguration(JSON_Object * module_obj);
	bool LoadConfiguration(JSON_Object * module_obj);

	void OnResize(int width, int height);

	void StartSceneRender();
	void EndSceneRender();
	
	void DrawConfigurationUi();
public:
	Light lights[MAX_LIGHTS];
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, projection_matrix;

	uint frame_buffer = 0u;
	uint render_texture = 0u;
	uint depth_render_buffer = 0u;
	uint stencil_buffer = 0u;

	float camera_near = 1.f;
	float camera_far = 1000.f;
	float fov = 60.f;
	float background_col[3];

	friend class PanelConfiguration;
};

#endif