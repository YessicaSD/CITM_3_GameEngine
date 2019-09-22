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
	sphere1.pos = { 0,0,0 };
	sphere1.r = 5;

	sphere2.pos = { 0,0,0 };
	sphere2.r = 3;

	pos = { 5,2,3 };

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleScene::Update(float dt)
{
	if (sphere1.Intersects(sphere2))
	{
		LOG("They intersect");
		LOG("%f, %f, %f", pos.x, pos.y, pos.z);
	}
	return UPDATE_CONTINUE;
}

