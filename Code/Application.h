#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"


class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera0 = nullptr;
	ModuleCamera3D* camera1 = nullptr;
	ModuleCamera3D* camera2 = nullptr;
	ModuleCamera3D* camera3 = nullptr;
	ModulePhysics3D* physics = nullptr;

private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	//Additional methods
	bool DrawAll();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};