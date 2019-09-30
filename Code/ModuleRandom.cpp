#include "ModuleRandom.h"
#include "Module.h"
#include "Application.h"

ModuleRandom::ModuleRandom(bool start_enabled) : Module(start_enabled)
{
}

bool ModuleRandom::Init()
{
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
	std::uniform_int_distribution<uint> uniform_dist(0, INT_MAX);
	return (float)uniform_dist(rng) / (float)INT_MAX;
}

float ModuleRandom::RandomFloat(float from, float to)
{
	//We may be able to get more precision by using something different than INT_MAX
	std::uniform_int_distribution<uint> uniform_dist(0, INT_MAX);
	return ((float)uniform_dist(rng) / (float)INT_MAX) * (to - from) + from;
};
