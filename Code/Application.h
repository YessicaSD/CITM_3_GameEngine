#pragma once


#include "Globals.h"
#include "Timer.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleGui.h"
#include "ModuleRandom.h"

#include <vector>
#include <list>
#include "MathGeoLib\include\Time\Clock.h"

typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

class ModuleImportFBX;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleScene* scene = nullptr;
	ModuleGui* gui = nullptr;
	ModuleRandom * random = nullptr;
	ModuleImportFBX* importFBX = nullptr;

	bool saveRequest = false;
	JSON_Object * config = nullptr;

private:
	//fps and timers variables 
	Timer				startup_time;
	uint64				frame_count = 0;
	float avg_fps		= 0.0f;
	float seconds_since_startup = 0.0f;


	Timer	ms_timer;
	float	dt;
	std::vector<Module*> list_modules;

	JSON_Value * configValue = nullptr;

	std::list<std::string> log_strings;
	
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* path);

	float GetAvgFPS() const
	{
		return avg_fps;
	}
	uint GetLastFrameMs()
	{
		return ms_timer.Read();
	}
	void Log(const char* sentece);
	
	void ChangeRenderMode(std::string variable);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void SaveModules();

	//Config
	void LoadConfig();
	void CloseConfig();
	void CreateNewConfig(const std::string& path);
	std::string config_path;
};

extern Application * App;