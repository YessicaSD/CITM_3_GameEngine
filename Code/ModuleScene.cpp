#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"

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
	LOG("random number: %f", App->random->RandomFloat(0.f, 100.f));
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	PPlane p(0, 1, 0, 0);
	p.axis = true;
	p.wire = false;
	p.Render();
	return UPDATE_CONTINUE;
}

