#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Application.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		DecideGLAndGLSLVersions();
		ret = SetWindow();
	}

	return ret;
}

// Create window with graphics context
bool ModuleWindow::SetWindow()
{
	bool ret = true;

	int width = SCREEN_WIDTH * SCREEN_SIZE;
	int height = SCREEN_HEIGHT * SCREEN_SIZE;
	Uint32 flags = GetFlags();

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	if (window == NULL)
	{
		LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		screen_surface = SDL_GetWindowSurface(window);
	}

	return ret;
}

void ModuleWindow::DecideGLAndGLSLVersions()
{
#if __APPLE__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
}

Uint32 ModuleWindow::GetFlags()
{
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

	if (WIN_FULLSCREEN == true)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	if (WIN_RESIZABLE == true)
	{
		flags |= SDL_WINDOW_RESIZABLE;
	}

	if (WIN_BORDERLESS == true)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}

	if (WIN_FULLSCREEN_DESKTOP == true)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	return flags;
}

update_status ModuleWindow::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_UP)==KEY_REPEAT)
	{
		int  x, y;
		SDL_GetWindowPosition(window, &x, &y);
		SDL_SetWindowPosition(window, x, y-1);
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleWindow::PostUpdate(float dt) {
	char title[160];
	sprintf_s(title, "Engine^^");
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

//Change when we get iPoint / Vector2
int ModuleWindow::GetWindowWidth()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return w;
}

int ModuleWindow::GetWindowHeight()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return h;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}