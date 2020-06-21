#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "../SDL/include/SDL.h"
#include "../parson/parson.h"

#define MY_BUFSIZE 1024 // Buffer size for console window titles.

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(const char* name ,bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JSONFile * module_file) override;

	bool CleanUp();

	void SetTitle(const char* title);

	int GetWindowWidth();
	int GetWindowHeight();
	
	void SetWidth(float& width);
	void SetHeight(float& height);
	void SetWindowSize(float width, float height);

	bool IsVsync();
	
	void SetBrightness(float brightness);
	
	void GetMaxWindowSize(float& width, float& height);


	void DrawConfigurationUi();

	void SetFullscreenMode();
	
	bool SaveConfiguration(JSONFile * module_file) override;
	bool LoadConfiguration(JSONFile * module_file) override;

private:
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
	//This values will be overriden by config_root.json
	bool resizable = false;
	bool borderless = false;
	bool vsync = false;
	int current_window_mode = 0;

	float brightness = 100.f;
	float max_width, max_height, min_width = 640, min_height = 480;

};

#endif // __ModuleWindow_H__