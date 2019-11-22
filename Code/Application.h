#ifndef __APLICATION_H__
#define __APLICATION_H__

#include <vector>
#include <list>

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "parson/parson.h"
#include "JSONFile.h"

#define FPS_GRAPH_SAMPLES 101
#define GRAPH_TITLE_SIZE 100

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
class ModuleResourceManager;

struct ImVec2;

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
	ModuleResourceManager * resource_manager = nullptr;

	std::string application_name;
	std::string organization_name;

private:
	std::vector<Module*> modules;

	JSONFile config;

	std::list<std::string> log_strings;

	//Framerate calculations

	//- dt
	PerfTimer curr_frame_time;
	float dt = 0.0f;

	//- average fps
	float avg_fps = 0.f;
	uint64 frame_count = 0u;

	//- cap fps
	uint32 curr_frame_ms = 0u;
	uint32 cap_time = 0u;
	float max_fps = 0.0f;
	bool cap_fps = false;

	//- fps last seocond
	Timer last_second_timer;
	uint32 last_second_fps = 0u;

	//- startup time
	Timer startup_time;
	float seconds_since_startup = 0.0f;

	//grahs

	const uint graph_title_size = 100u;

	//- fps graph
	int fps_graph_index = 0;
	float fps_history[FPS_GRAPH_SAMPLES];

	//- ms graph
	int ms_graph_index = 0;
	float ms_history[FPS_GRAPH_SAMPLES];

	//- ram graph
	float ram_history[FPS_GRAPH_SAMPLES];

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool DrawAppConfigUI();

	bool LoadModulesConfiguration();
	bool SaveModulesConfiguration();
	bool LoadModulesConfigurationWithOpenFile();
	bool CleanUp();

	void RequestBrowser(const char* path);

	void Log(const char* sentece);
	void EventRequest(const Event& event);
	void DrawModulesConfigUi();
	void AddEvent(const Event& event);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	//Config
	bool LoadAppConfiguration(JSONFile * app_file);
	bool SaveAppConfiguration(JSONFile * app_file);

	void UpdateFPSGraph(uint32 last_second_fps);
	void DrawFPSGraph(char * titleGraph, const ImVec2 &size);

	void UpateMsGraph(uint32 curr_frame_ms);
	void DrawMsGraph(char * titleGraph, const ImVec2 &size);

	void PopEventsInQueue();

	std::vector<Event> event_queue;
	std::string config_path;
};

extern Application * App;

#endif