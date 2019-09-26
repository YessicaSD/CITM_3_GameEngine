#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));	
	
	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	return true;
}

// Update: draw background
update_status ModuleScene::Update(float dt)
{
	// Make a random number engine
	static pcg32 rng(seed_source);

	// Choose a random mean between 1 and 6
	std::uniform_int_distribution<int> uniform_dist(1, 6);
	int mean = uniform_dist(rng);
	LOG("Randomly-chosen mean: %i \n", mean);

	return UPDATE_CONTINUE;
}

