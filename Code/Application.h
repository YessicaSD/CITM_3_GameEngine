#ifndef __APLICATION_H__
#define __APLICATION_H__

#include <vector>
#include <list>

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "parson/parson.h"

#define FPS_GRAPH_SAMPLES 101

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

	std::string application_name;
	std::string organization_name;

private:
	std::vector<Module*> modules;

	JSON_Value * configValue = nullptr;

	std::list<std::string> log_strings;

	//Framerate calculations

	//- dt
	PerfTimer frame_time;
	float dt = 0.0f;

	//- last frame
	uint32 last_frame_ms = 0u;

	//- average fps
	float avg_fps = 0.f;
	uint64 frame_count = 0u;

	//- cap frames
	uint32 cap_time = 0u;
	float max_fps = 0.0f;
	bool cap_fps = false;

	//- frames last second
	Timer last_sec_frame_time;
	uint32 last_sec_frame_count = 0u;

	//- startup time
	Timer startup_time;
	float seconds_since_startup = 0.0f;

	//grahs
	Timer updateGraph;
	float fpsHistory[FPS_GRAPH_SAMPLES];
	float msHistory[FPS_GRAPH_SAMPLES];
	float RamHistory[FPS_GRAPH_SAMPLES];

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool DrawAppConfigUI();
	bool LoadModulesConfiguration();
	bool SaveModulesConfiguration();
	bool CleanUp();

	void RequestBrowser(const char* path);

	float GetAvgFPS() const;
	uint GetLastFrameMs();
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