#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

#define MY_BUFSIZE 1024 // Buffer size for console window titles.

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void SetTitle(const char* title);

private:
	Uint32 GetFlags();
	void DecideGLAndGLSLVersions();
	bool SetWindow();

public:
	int GetWindowWidth();
	int GetWindowHeight();

	//The window we'll be rendering to
	SDL_Window* window = nullptr;
	SDL_GLContext gl_context;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;
	const char* glsl_version;
};

#endif // __ModuleWindow_H__