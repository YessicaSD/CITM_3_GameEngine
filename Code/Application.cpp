#include "Application.h"
#include <Windows.h>
#include "parson/parson.h"
#include "ModuleImport.h"
#include "ModuleTexture.h"
#include "ModuleFileSystem.h"
#include "ModuleAudio.h"
#include "ModuleHardware.h"
#include "imgui/imgui.h"
#include "Event.h"
Application::Application()
{
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window = new ModuleWindow("Window"));
	AddModule(random = new ModuleRandom());
	AddModule(file_system = new ModuleFileSystem());
	AddModule(hardware = new ModuleHardware("Hardware"));
	AddModule(input = new ModuleInput("Input"));
	AddModule(texture = new ModuleTexture());
	AddModule(scene = new ModuleScene());
	AddModule(import = new ModuleImport());
	AddModule(camera = new ModuleCamera3D());
	AddModule(audio = new ModuleAudio("Audio"));
	AddModule(gui = new ModuleGui());

	// Renderer last!
	AddModule(renderer3D = new ModuleRenderer3D("Render"));

}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete (*item);
		item = ++item;
	}
}

bool Application::Init()
{
	bool ret = true;

	config_path = "config.json";

	LoadConfig();

	// Call Init() in all modules
	std::vector<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		if ((*item)->IsActive())
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true )
	{
		if ((*item)->IsActive())
		ret = (*item)->Start();
		++item;
	}
	
	CloseConfig();

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();

	if (log_strings.size() > 0)
	{
		if (gui)
		{
			for (auto iter = log_strings.begin(); iter != log_strings.end(); ++iter)
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
	if (saveRequest)
	{
		SaveModules();
		saveRequest = false;
	}

	frame_count++;

	seconds_since_startup = startup_time.ReadSec();
	avg_fps = float(frame_count) / seconds_since_startup;

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::vector<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if((*item)->IsActive())
			ret = (*item)->PreUpdate();
		item = ++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsActive())
		ret = (*item)->Update(dt);
		item = ++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsActive())
			ret = (*item)->PostUpdate();
		item = ++item;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::vector<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend() && ret == true)
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
	for (std::vector<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end(); ++iter)
	{
		(*iter)->EventRequest(event);
	}
}

void Application::DrawModulesConfigUi()
{
	for (std::vector<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end(); ++iter)
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
	list_modules.push_back(mod);
}

void Application::SaveModules()
{
	bool ret = true;

	//Create new config file if there isn't one
	if (config == nullptr)
	{
		CreateNewConfig(config_path.c_str());
	}

	for (std::vector<Module*>::iterator item = list_modules.begin();
		item != list_modules.end() && ret == true;
		item = ++item)
	{
		ret = (*item)->Save(config);
	}
}

//Config

void Application::LoadConfig()
{
	configValue = json_parse_file(config_path.c_str());
	config = json_object(configValue);
}

void Application::CloseConfig()
{
	json_serialize_to_file_pretty(configValue, config_path.c_str());
	json_value_free(configValue);
	config = nullptr;
	configValue = nullptr;
}

void Application::CreateNewConfig(const std::string& path)
{
	configValue = json_value_init_object();
	config = json_value_get_object(configValue);
	json_serialize_to_file_pretty(configValue, path.data());
	if (configValue == nullptr || config == nullptr)
	{
		LOG("Error creating JSON with path %s", path.data());
	}
}