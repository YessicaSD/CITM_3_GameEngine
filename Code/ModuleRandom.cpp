#include "ModuleRandom.h"

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

float ModuleRandom::RandomFloat(float from, float to)
{
	std::uniform_int_distribution<float> uniform_dist(from, to);
	return uniform_dist(rng);
};
