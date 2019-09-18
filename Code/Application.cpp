#include "Application.h"


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	physics = new ModulePhysics3D(this);
	player0 = new ModulePlayer(this, 0);
	player1 = new ModulePlayer(this, 1);
	player2 = new ModulePlayer(this, 2);
	player3 = new ModulePlayer(this, 3);

	player0->color_Chassis = { 1.f,32/255.f,148 / 255.f,1 };
	player1->color_Chassis = { 0,1,1,1 };
	player2->color_Chassis = { 0,1,112 / 255,1 };
	player3->color_Chassis = { 124/255.f,53 / 255.f,1.f,1 };
	camera0 = new ModuleCamera3D(this, player0);
	camera1 = new ModuleCamera3D(this, player1);
	camera2 = new ModuleCamera3D(this, player2);
	camera3 = new ModuleCamera3D(this, player3);

	
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	// Scenes
	AddModule(scene_intro);
	//Players
	AddModule(player0);
	AddModule(player1);
	AddModule(player2);
	AddModule(player3);
	//Cameras
	AddModule(camera0);
	AddModule(camera1);
	AddModule(camera2);
	AddModule(camera3);
	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
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
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

bool Application::DrawAll()
{
	bool ret = true;

	for (p2List_item<Module*>* item = list_modules.getFirst(); item != NULL && ret; item = item->next) {
		ret = item->data->Draw();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}