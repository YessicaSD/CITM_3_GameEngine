#include "Application.h"
#include <Windows.h>
#include "parson/parson.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	scene = new ModuleScene();
	gui = new ModuleGUI();
	random = new ModuleRandom();
	editor = new ModuleEditor();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(random);
	AddModule(input);
	AddModule(scene);
	AddModule(editor);
	AddModule(gui);
	AddModule(camera);
	// Renderer last!
	AddModule(renderer3D);
	
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

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
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
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
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate();
		item = ++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item = ++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate();
		item = ++item;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item = ++item;
	}
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

	for (std::list<Module*>::iterator item = list_modules.begin();
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