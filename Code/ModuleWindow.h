#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "parson/parson.h"

#define MY_BUFSIZE 1024 // Buffer size for console window titles.

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(const char* name ,bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init() override;

	bool CleanUp();

	void SetTitle(const char* title);

	bool Save(JSON_Object * config);

	int GetWindowWidth();
	
	int GetWindowHeight();
	
	bool IsVsync();
	
	void SetBrightness(float brightness);
	
	void GetMaxWindowSize(float& width, float& hight);
	
	void SetWidth(float& width);

	void SetHeight(float& height);

	void DrawConfigurationUi();
private:
	void LoadConfigValues(JSON_Object * config);
	Uint32 GetFlags();
	void DecideGLAndGLSLVersions();
	bool SetWindow();
	
	

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;
	SDL_GLContext gl_context;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;
	const char* glsl_version = nullptr;


	//TODO: Make them private
	//This values will be overriden by config.json
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;
	bool vsync = false;
};

#endif // __ModuleWindow_H__