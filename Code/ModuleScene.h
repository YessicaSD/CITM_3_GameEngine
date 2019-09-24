#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#include "Module.h"
#include "Globals.h"
#include <random>
#include "pcg-cpp-0.98\include\pcg_random.hpp"

class ModuleScene : public Module
{
private:
	// Seed with a real random value, if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	

public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
};

#endif // !MODULESCENE_H_

