#ifndef MODULESCENE01_H_
#define MODULESCENE01_H_

#include "Module.h"
#include "Globals.h"

#include "MathGeoLib\include\Geometry\Sphere.h"
#include "MathGeoLib\include\Math\float3.h"
class ModuleScene01 : public Module
{
public:
	ModuleScene01(Application* app, bool start_enabled = true);
	~ModuleScene01();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	Sphere sphere1, sphere2;
	float3 pos;

public:
};

#endif // !MODULESCENE01_H_

