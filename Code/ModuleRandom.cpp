#include "ModuleRandom.h"
#include "Module.h"
#include "Application.h"

ModuleRandom::ModuleRandom(const char * name, bool start_enabled) : Module(start_enabled, name)
{
}

bool ModuleRandom::Init(JSONFile * module_file)
{
	LOG("Initializing random seed source from Module Random");
	// Seed with a real random value, if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	// Make a random number engine
	//rng = new pcg32 (seed_source);
	rng.seed(seed_source);
	return true;
}

int ModuleRandom::RandomInt(int from, int to)
{
	std::uniform_int_distribution<int> uniform_dist(from, to);
	return uniform_dist(rng);
};

float ModuleRandom::RandomFloat0to1()
{
	std::uniform_real_distribution<float> uniform_dist(0.f, 1.f);
	return uniform_dist(rng);
}

UID ModuleRandom::RandomUID()
{
	//INFO: We don't use 0 because we consider it INVALID_RESOURCE_UID and INVALID_GAMEOBJECT_UID
	std::uniform_int_distribution<UID> uniform_dist(1, ULLONG_MAX);
	return uniform_dist(rng);
}

float ModuleRandom::RandomFloat(float from, float to)
{
	std::uniform_real_distribution<float> uniform_dist(from, to);
	return uniform_dist(rng);
};
