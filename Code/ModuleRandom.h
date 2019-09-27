#ifndef __MODULE_RANDOM_H__
#define __MODULE_RANDOM_H__

#include"Module.h"
#include <random>
#include "pcg-cpp-0.98\include\pcg_random.hpp"

class ModuleRandom : public Module
{
private:
	bool Init() override;

public:
	int RandomInt(int from, int to);
	float RandomFloat(float from, float to);
private:
	pcg32 rng;
};

#endif
