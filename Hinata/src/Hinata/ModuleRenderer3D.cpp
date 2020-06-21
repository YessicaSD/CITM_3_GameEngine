#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGui.h"
#include "PanelScene.h"

#include "../glew/include/GL/glew.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "ModuleFileSystem.h"
#include "JSONFile.h"
#include "PanelConfiguration.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"


ModuleRenderer3D::ModuleRenderer3D(const char* name, bool start_enabled) : Module(start_enabled, name)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
}

// Called before render is available
bool ModuleRenderer3D::Init(JSONFile * module_file)
{
	bool ret = true;

	// Initialize OpenGL loader
	if (glewInit() != GLEW_OK)
	{
		//fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		LOG("Failed to initialize OpenGL loader!\n");
		ret = false;
	}
	else
	{
		LOG("Successful initialization of GLEW");
	}

	if(ret)
	{
		LOG("Initializing module render");
		//Use Vsync
		if (App->window->IsVsync() && SDL_GL_SetSwapInterval(1) < 0)
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", );
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(3 / 255.F, 19 / 255.F, 29 / 255.F,1);
		//(57 / 255.F, 138 / 255.F, 209 / 255.F);
		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		lights[1].ref = GL_LIGHT0;
		lights[1].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[1].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[1].SetPos(0.0f, 0.0f, 2.5f);
		lights[1].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		lights[1].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//Render options
	depth_test = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	cull_face = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
	lighting = glIsEnabled(GL_LIGHTING) == GL_TRUE;
	color_material = glIsEnabled(GL_COLOR_MATERIAL) == GL_TRUE;
	texture_2d = glIsEnabled(GL_TEXTURE_2D) == GL_TRUE;

	scene_fbo.GenerateFrameBuffer();
	game_fbo.GenerateFrameBuffer();
	
	return ret;
}

void ModuleRenderer3D::UpdateProjectionMatrix()
{
	//TODO: MAKE IT USE THE SAME MATRIX AS RENDER TEXTURE 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf((GLfloat*)&App->camera->current_camera->GetProjectionMatrix().Transposed());


}

bool ModuleRenderer3D::Start(JSONFile * module_file)
{
	UpdateProjectionMatrix();
	return true;
}


// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate()
{
	if (App->camera->current_camera->update_project_matrix)
	{
		UpdateProjectionMatrix();
		App->camera->current_camera->update_project_matrix = false;
	}
	// light 0 on cam pos
	float* pos = (float*)&App->camera->GetPos();
	lights[0].SetPos(pos[0] , pos[1], pos[2]);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
	{
		lights[i].Render();
	}

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	glDeleteTextures(1, &scene_fbo.render_texture);
	glDeleteRenderbuffers(1, &scene_fbo.depth_render_buffer);
	glDeleteFramebuffers(1, &scene_fbo.frame_buffer);

	SDL_GL_DeleteContext(App->window->gl_context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{

}

bool ModuleRenderer3D::SaveConfiguration(JSONFile * module_file)
{
	
	module_file->SaveBool("depth test", depth_test);
	module_file->SaveBool("cull faces", cull_face);
	module_file->SaveBool("lightning", lighting);
	module_file->SaveBool("color materials", color_material);
	module_file->SaveBool("texture 2d", texture_2d);

	return true;
}

bool ModuleRenderer3D::LoadConfiguration(JSONFile * module_file)
{
	
	

	depth_test = module_file->LoadBool("depth test");
	if (depth_test)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	cull_face = module_file->LoadBool("cull faces");
	if (depth_test)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	lighting = module_file->LoadBool("lightning");
	if (depth_test)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	color_material = module_file->LoadBool("color material");
	if (depth_test)
	{
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	texture_2d = module_file->LoadBool("texture 2d");
	if (depth_test)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	return true;
}