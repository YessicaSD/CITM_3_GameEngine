#ifndef __APLICATION_H__
#define __APLICATION_H__

#include <vector>
#include <list>

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "parson/parson.h"

typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

class Module;
class ModuleImport;
class ModuleTexture;
class ModuleFileSystem;
class ModuleAudio;
class ModuleHardware;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleScene;
class ModuleGui;
class ModuleRandom;

struct Event;

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
	ModuleImport* import = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleFileSystem* file_system = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleHardware* hardware = nullptr;
	
	JSON_Object * config_root = nullptr;

private:
	//fps and timers variables 
	Timer				startup_time;
	uint64				frame_count = 0;
	float avg_fps		= 0.0f;
	float seconds_since_startup = 0.0f;


	PerfTimer ms_timer;
	float	dt;
	std::vector<Module*> modules;

	JSON_Value * configValue = nullptr;

	std::list<std::string> log_strings;
	
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool LoadModulesConfiguration();
	bool SaveModulesConfiguration();
	bool CleanUp();

	void RequestBrowser(const char* path);

	float GetAvgFPS() const
	{
		return avg_fps;
	}
	uint GetLastFrameMs()
	{
		return ms_timer.ReadMs();
	}
	void Log(const char* sentece);
	void EventRequest(const Event& event);
	void DrawModulesConfigUi();
private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	//Config
	void LoadConfig();
	void CloseConfig();
	void CreateNewConfig(const std::string& path);
	std::string config_path;
};

extern Application * App;

#endif