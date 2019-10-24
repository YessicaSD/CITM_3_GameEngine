#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "glew/include/GL/glew.h"
#include "imgui/imgui_impl_opengl3.h"


ModuleRenderer3D::ModuleRenderer3D(const char* name, bool start_enabled) : Module(start_enabled, name)
{

}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
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
	depth_test = glIsEnabled(GL_DEPTH_BUFFER) == GL_TRUE;
	cull_face = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
	lighting = glIsEnabled(GL_LIGHTING) == GL_TRUE;
	color_material = glIsEnabled(GL_COLOR_MATERIAL) == GL_TRUE;
	texture_2d = glIsEnabled(GL_TEXTURE_2D) == GL_TRUE;

	GenSceneFramebuffer();

	return ret;
}

void ModuleRenderer3D::GenSceneFramebuffer()
{
	//https://learnopengl.com/Advanced-OpenGL/Framebuffers
	//Generate frame buffer
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

	//Generate depth render buffer
	glGenRenderbuffers(1, &depth_render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer);
	//Attach to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer);
	//Reset depth render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Generate render texture
	glGenTextures(1, &render_texture);
	glBindTexture(GL_TEXTURE_2D, render_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//Attach to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);
	//Reset render texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//Reset framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate()
{
	//TODO: If this is updated in ModuleInput->PreUpdate we maybe should change the viewport size after
	ImVec2 size = App->gui->current_viewport_size;

	PrepareCamera(size);
	PrepareDepthBuffer(size);
	PrepareTextureBuffer(size);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("[Error] creating screen buffer");
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
	{
		lights[i].Render();
	}

	return UPDATE_CONTINUE;
}


void ModuleRenderer3D::PrepareCamera(ImVec2 &size)
{
	glViewport(0, 0, size.x, size.y);

	glMatrixMode(GL_PROJECTION);
	projection_matrix = perspective(60.0f, size.x / size.y, camera_near, camera_far);
	glLoadMatrixf(&projection_matrix);

	//Reset camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::PrepareDepthBuffer(ImVec2 &size)
{
	glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	
	//Reset buffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void ModuleRenderer3D::PrepareTextureBuffer(ImVec2 &size)
{
	glBindTexture(GL_TEXTURE_2D, render_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	//Reset buffer
	glBindTexture(GL_TEXTURE_2D, 0);
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

	glDeleteTextures(1, &render_texture);
	glDeleteRenderbuffers(1, &depth_render_buffer);
	glDeleteFramebuffers(1, &frame_buffer);

	SDL_GL_DeleteContext(App->window->gl_context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	projection_matrix = perspective(fov, (float)width / (float)height, camera_near, camera_far);
	glLoadMatrixf(&projection_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::StartSceneRender()
{
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	glBindFramebuffer(GL_FRAMEBUFFER, App->renderer3D->frame_buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Stencil
	glStencilFunc(GL_ALWAYS, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void ModuleRenderer3D::EndSceneRender()
{
	//Stencil
	glStencilFunc(GL_ALWAYS, 1, 0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, App->renderer3D->render_texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}