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
	AddModule(random = new ModuleRandom("Random"));
	AddModule(file_system = new ModuleFileSystem("File System"));
	AddModule(hardware = new ModuleHardware("Hardware"));
	AddModule(input = new ModuleInput("Input"));
	AddModule(texture = new ModuleTexture("Textures"));
	AddModule(scene = new ModuleScene("Scene"));
	AddModule(import = new ModuleImport("Import"));
	AddModule(camera = new ModuleCamera3D("Camera 3D"));
	AddModule(audio = new ModuleAudio("Audio"));
	AddModule(gui = new ModuleGui("Gui"));

	// Renderer last!
	AddModule(renderer3D = new ModuleRenderer3D("Render"));

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
	LoadConfig();
	
	// Call Init() in all modules
	std::vector<Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		if ((*item)->IsActive())
			ret = (*item)->Init(config_root);
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		if ((*item)->IsActive())
			ret = (*item)->Start(config_root);
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
	frame_count++;

	seconds_since_startup = startup_time.ReadSec();
	avg_fps = float(frame_count) / seconds_since_startup;

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

bool Application::SaveModulesConfiguration()
{
	bool ret = true;

	//When saving we override the previous file
	CreateNewConfig(config_path);

	json_object_set_value(config_root, "App", json_value_init_object());
	JSON_Object * app_obj = json_object_get_object(config_root, "App");
	
	for (std::vector<Module*>::iterator item = modules.begin();
		item != modules.end() && ret;
		item = ++item)
	{
		json_object_set_value(app_obj, (*item)->name, json_value_init_object());
		JSON_Object * module_obj = json_object_get_object(app_obj, (*item)->name);
		ret = (*item)->SaveConfiguration(module_obj);
	}

	json_serialize_to_file_pretty(configValue, config_path.data());
	
	CloseConfig();

	if (ret)
	{
		LOG("Saved configuration successfully.");
	}

	return ret;
}

bool Application::LoadModulesConfiguration()
{
	bool ret = true;

	LoadConfig();
	JSON_Object * app_obj = json_object_get_object(config_root, "App");

	if(config_root != nullptr)
	{
		for (std::vector<Module*>::iterator item = modules.begin();
			item != modules.end() && ret == true;
			++item)
		{
			ret = (*item)->LoadConfiguration(config_root);
			JSON_Object * module_obj = json_object_get_object(app_obj, (*item)->name);
			ret = (*item)->LoadConfiguration(module_obj);
		}
	}
	CloseConfig();

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

//Config

void Application::LoadConfig()
{
	configValue = json_parse_file(config_path.c_str());
	config_root = json_object(configValue);
}

void Application::CloseConfig()
{
	json_value_free(configValue);
	config_root = nullptr;
	configValue = nullptr;
}

void Application::CreateNewConfig(const std::string& path)
{
	configValue = json_value_init_object();
	config_root = json_value_get_object(configValue);
	if (configValue == nullptr || config_root == nullptr)
	{
		LOG("Error creating JSON with path %s", path.data());
	}
}