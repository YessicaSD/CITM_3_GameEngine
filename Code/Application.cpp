#include "Application.h"

#include <Windows.h>
#include "parson/parson.h"
#include "Event.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "JSONFile.h"

#include "ModuleImport.h"
#include "ModuleTexture.h"
#include "ModuleFileSystem.h"
#include "ModuleAudio.h"
#include "ModuleHardware.h"
#include "ModuleWindow.h"
#include "ModuleRandom.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleGui.h"
#include "ModuleRenderer3D.h"
#include "ModuleResourceManager.h"

Application::Application()
{
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window = new ModuleWindow("Window"));
	AddModule(random = new ModuleRandom("Random"));
	AddModule(file_system = new ModuleFileSystem("File System"));
	AddModule(hardware = new ModuleHardware("Hardware"));
	AddModule(input = new ModuleInput("Input"));
	AddModule(texture = new ModuleTexture("Textures"));
	AddModule(scene = new ModuleScene("Scene"));
	AddModule(import = new ModuleImport("Import"));
	AddModule(camera = new ModuleCamera3D("Camera 3D"));
	//AddModule(audio = new ModuleAudio("Audio"));
	AddModule(gui = new ModuleGui("Gui"));
	AddModule(resource_manager = new ModuleResourceManager("Resource Manager"));

	// Renderer last!
	AddModule(renderer3D = new ModuleRenderer3D("Render"));

	memset(fps_history, 0, sizeof(float) * FPS_GRAPH_SAMPLES);
	memset(ms_history, 0, sizeof(float) * FPS_GRAPH_SAMPLES);
	memset(ram_history, 0, sizeof(float) * FPS_GRAPH_SAMPLES);
}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator item = modules.rbegin();

	while (item != modules.rend())
	{
		delete (*item);
		item = ++item;
	}
}

bool Application::Init()
{
	bool ret = true;

	config_path = "config.json";

	//Automatically load the config file if it exists
	config.LoadFile(config_path);
	config.AddSection("App");

	//TODO: Load app values
	//cap frames
	//max fps

	// Call Init() in all modules
	std::vector<Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		if ((*item)->IsActive())
		{
			ret = (*item)->Init(&config);
		}
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		if ((*item)->IsActive())
		{
			ret = (*item)->Start(&config);
		}
		++item;
	}

	ret = LoadModulesConfigurationWithOpenFile();

	config.CloseFile();

	//Framerate calculations
	cap_time = (uint32)(1000 / max_fps);
	curr_frame_time.Start();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	//Calculate dt
	dt = (float)curr_frame_time.ReadMs() / 1000.0f;
	curr_frame_time.Start();

	if (log_strings.size() > 0)
	{
		if (gui)
		{
			for (std::list<std::string>::iterator iter = log_strings.begin();
				iter != log_strings.end();
				++iter)
			{
				if (!gui->Log((*iter).c_str()))
				{
					return;
				}
			}
			log_strings.clear();
		}
	}
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	//Framerate calculations
	frame_count++;
	last_second_fps++;
	if (last_second_timer.Read() > 1000)
	{
		UpdateFPSGraph(last_second_fps);
		last_second_fps = 0;
		last_second_timer.Start();
	}
	seconds_since_startup = startup_time.ReadSec();
	avg_fps = float(frame_count) / seconds_since_startup;
	curr_frame_ms = (uint32)curr_frame_time.ReadMs();
	UpateMsGraph(curr_frame_ms);

	//Cap fps
	if (cap_fps)
	{
		uint32 delay = MAX(0, (int)cap_time - (int)curr_frame_ms);
		if (delay > 0)
		{
			SDL_Delay(delay);
		}
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::vector<Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsActive())
			ret = (*item)->PreUpdate();
		item = ++item;
	}

	item = modules.begin();

	while (item != modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsActive())
			ret = (*item)->Update(dt);
		item = ++item;
	}

	item = modules.begin();

	while (item != modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsActive())
			ret = (*item)->PostUpdate();
		item = ++item;
	}

	FinishUpdate();
	return ret;
}

bool Application::DrawAppConfigUI()
{
	//Project name
	if (ImGui::InputText("Application Name:", &application_name))
	{
		App->window->SetTitle(App->application_name.c_str());
	}
	ImGui::InputText("Organization:", &organization_name);

	//FPS
	ImGui::Checkbox("Cap FPS", &cap_fps);
	if (cap_fps)
	{
		if (ImGui::SliderFloat("Max FPS", &max_fps, 0.0f, 60.0f, "%.0f"))
		{
			cap_time = 1000 / max_fps;
		}
	}

	ImVec2 size = { 310,100 };
	char titleGraph[GRAPH_TITLE_SIZE];
	DrawFPSGraph(titleGraph, size);
	DrawMsGraph(titleGraph, size);

	return true;
}

void Application::UpdateFPSGraph(uint32 last_second_fps)
{
	fps_history[fps_graph_index] = last_second_fps;

	++fps_graph_index;
	if (fps_graph_index == FPS_GRAPH_SAMPLES)
	{
		fps_graph_index = 0;
	}
}

void Application::DrawFPSGraph(char * titleGraph, const ImVec2 &size)
{
	uint32 last_index = fps_graph_index - 1;
	if (last_index == -1)
	{
		last_index = FPS_GRAPH_SAMPLES - 1;
	}

	sprintf_s(titleGraph, GRAPH_TITLE_SIZE, "Framerate: %.2f", fps_history[last_index]);
	ImGui::PlotHistogram("##ASDFASF", fps_history, IM_ARRAYSIZE(fps_history), fps_graph_index, titleGraph, 0.0f, 100.0f, size);
}

void Application::UpateMsGraph(uint32 curr_frame_ms)
{
	ms_history[ms_graph_index] = curr_frame_ms;

	++ms_graph_index;
	if (ms_graph_index == FPS_GRAPH_SAMPLES)
	{
		ms_graph_index = 0;
	}
}

void Application::DrawMsGraph(char * titleGraph, const ImVec2 &size)
{
	uint32 last_index = ms_graph_index - 1;
	if (last_index == -1)
	{
		last_index = FPS_GRAPH_SAMPLES - 1;
	}

	sprintf_s(titleGraph, GRAPH_TITLE_SIZE, "Milliseconds: %i", ms_history[last_index]);
	ImGui::PlotHistogram("##ASDFASF", ms_history, IM_ARRAYSIZE(ms_history), ms_graph_index, titleGraph, 0.0f, 15.0f, size);
}

bool Application::SaveModulesConfiguration()
{
	bool ret = true;

	//When saving we override the previous file
	config.CreateJSONFile(config_path);
	JSONFile * app_file = &config.AddSection("App");
	SaveAppConfiguration(app_file);

	for (std::vector<Module*>::iterator item = modules.begin();
		item != modules.end() && ret;
		item = ++item)
	{
		JSONFile * module_file = &config.AddSection((*item)->name);
		if (module_file != nullptr)
		{
			ret = (*item)->SaveConfiguration(module_file);
		}
	}

	config.SaveFile(config_path);
	config.CloseFile();

	if (ret)
	{
		LOG("Saved configuration successfully.");
	}

	return ret;
}

bool Application::LoadModulesConfigurationWithOpenFile()
{
	bool ret = true;
	config.AddSection("App");

	LoadAppConfiguration(&config);

	for (std::vector<Module*>::iterator item = modules.begin();
		item != modules.end() && ret == true;
		++item)
	{
		JSONFile * module_file = &config.GetSection((*item)->name);
		if (module_file != nullptr)
		{
			ret = (*item)->LoadConfiguration(module_file);
		}
	}

	return ret;
}

bool Application::LoadAppConfiguration(JSONFile * app_file)
{
	if (app_file != nullptr)
	{
		application_name = app_file->LoadText("application name");
		organization_name = app_file->LoadText("organization name");
		cap_fps = app_file->LoadBool("cap fps");
		max_fps = app_file->LoadNumber("max fps");
	}
	return true;
}

bool Application::SaveAppConfiguration(JSONFile * app_file)
{
	if (app_file != nullptr)
	{
		app_file->SaveText("application name", application_name.c_str());
		app_file->SaveText("organization name", organization_name.c_str());
		app_file->SaveBool("cap fps", cap_fps);
		app_file->SaveNumber("max fps", max_fps);
	}
	return true;
}

bool Application::LoadModulesConfiguration()
{
	bool ret = true;

	config.LoadFile(config_path);
	ret = LoadModulesConfigurationWithOpenFile();
	config.CloseFile();

	if (ret)
	{
		LOG("Load configuration successfully.");
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::vector<Module*>::reverse_iterator item = modules.rbegin();
	while (item != modules.rend() && ret == true)
	{

		ret = (*item)->CleanUp();

		item = ++item;
	}
	gui = nullptr;
	return ret;
}

void Application::RequestBrowser(const char* path)
{
	ShellExecuteA(
		0,
		"Open",
		path,
		0,
		0,
		0
	);
}

void Application::Log(const char * sentece)
{
	log_strings.push_back(sentece);
}

void Application::EventRequest(const Event & event)
{
	for (std::vector<Module*>::iterator iter = modules.begin(); iter != modules.end(); ++iter)
	{
		(*iter)->EventRequest(event);
	}
}

void Application::DrawModulesConfigUi()
{
	for (std::vector<Module*>::iterator iter = modules.begin(); iter != modules.end(); ++iter)
	{
		if ((*iter)->name != "")
		{
			if (ImGui::CollapsingHeader((*iter)->name))
			{
				(*iter)->DrawConfigurationUi();
			}
		}
	}
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}