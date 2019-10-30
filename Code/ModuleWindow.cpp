#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Application.h"

ModuleWindow::ModuleWindow(const char* name, bool start_enabled) : Module(start_enabled, name)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JSON_Object* module_obj)
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

bool ModuleWindow::SaveConfiguration(JSON_Object * module_obj)
{
	json_object_set_number(module_obj, "brightness", brightness);
	json_object_set_number(module_obj, "width", GetWindowWidth());
	json_object_set_number(module_obj, "height", GetWindowHeight());
	json_object_set_number(module_obj, "fullscreen mode", current_window_mode);
	json_object_set_boolean(module_obj, "resizable", resizable);
	json_object_set_boolean(module_obj, "borderless", borderless);
	json_object_set_boolean(module_obj, "vsync", vsync);
	return true;
}

bool ModuleWindow::LoadConfiguration(JSON_Object * module_obj)
{
	brightness = json_object_get_number(module_obj, "brightness");

	//INFO: Loading the brightness makes the screen go completely dark, only showing your cursor
	//SetBrightness(brightness);

	SetWindowSize(
		json_object_get_number(module_obj, "width"),
		json_object_get_number(module_obj, "height"));

	current_window_mode = json_object_get_number(module_obj, "fullscreen mode");
	SetFullscreenMode();

	resizable = json_object_get_boolean(module_obj, "resizable");
	SDL_SetWindowResizable(App->window->window, App->window->resizable ? SDL_TRUE : SDL_FALSE);

	borderless = json_object_get_boolean(module_obj, "borderless");
	SDL_SetWindowBordered(App->window->window, App->window->borderless ? SDL_FALSE : SDL_TRUE);

	vsync = json_object_get_boolean(module_obj, "vsync");
	if (SDL_GL_SetSwapInterval(App->window->vsync ? 1 : 0) == -1)
	{
		LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}
	return true;
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

	if (current_window_mode == 1)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	else if (current_window_mode == 2)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if (resizable)
	{
		flags |= SDL_WINDOW_RESIZABLE;
	}

	if (borderless)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}


	return flags;
}

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

bool ModuleWindow::IsVsync()
{
	return vsync;
}

void ModuleWindow::SetBrightness(float brightness)
{
	SDL_SetWindowBrightness(window,brightness);
}

void ModuleWindow::GetMaxWindowSize(float & width, float & height)
{
	SDL_DisplayMode display_mode;
	if (SDL_GetDesktopDisplayMode(0, &display_mode) != 0)
	{
		LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else
	{
		width = display_mode.w;
		height = display_mode.h;
	}
}

void ModuleWindow::SetWidth(float & width)
{
	SDL_SetWindowSize(window, width, this->GetWindowHeight());
}

void ModuleWindow::SetHeight(float & height)
{
	SDL_SetWindowSize(window, this->GetWindowWidth(), height);
}

void ModuleWindow::SetWindowSize(float width, float height)
{
	SDL_SetWindowSize(window, width, height);
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


void ModuleWindow::SetFullscreenMode()
{
	Uint32 change_mode = 0u;
	if (current_window_mode == 0)
	{
		change_mode = 0;
	}
	else if (current_window_mode == 1)
	{
		change_mode = SDL_WINDOW_FULLSCREEN;
	}
	else if (current_window_mode == 2)
	{
		change_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	SDL_SetWindowFullscreen(App->window->window, change_mode);
}