#include "Globals.h"
#include "Application.h"
#include "ModuleScene01.h"

#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\MathGeoLibFwd.h"
#include "MathGeoLib\include\MathBuildConfig.h"
#include "MathGeoLib\include\Math\float3.h"

#include "MathGeoLib\include\Geometry\Sphere.h"

ModuleScene01::ModuleScene01(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene01::~ModuleScene01()
{}

// Load assets
bool ModuleScene01::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	return ret;
}

// Load assets
bool ModuleScene01::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleScene01::Update(float dt)
{
	//Plane p(0, 1, 0, 0);
	//p.axis = true;
	//p.Render();
	Sphere sphere1;
	sphere1.pos = { 0,0,0 };
	sphere1.r = 5;
	
	return UPDATE_CONTINUE;
}

