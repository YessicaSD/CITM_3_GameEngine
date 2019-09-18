#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Color.h"
#include "PhysBody3D.h"
#define MAX_SNAKE 2

class Pendulum
{
public:
	enum PARTS_ENUM
	{
		STATIC_PART = 0,
		DINAMIC_ARM = 1,
		MAX_PARTS
	};

	Cylinder staticPoint;
	Cube Arm;
	PhysBody3D* parts[MAX_PARTS];
public:
	Pendulum() {}

};

struct Cub
{
	Cube RenderMesh;
	PhysBody3D* rb= nullptr;
};
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	void AddCubes();
	void AddSpheres();
	void AddStartingPoint();
	void AddCheckpoints();
	void AddGround();
	void AddTrack(int * trackOutline, int maxPos);
	void AddRamps();
	//p2DynArray<PhysBody3D*> AddBridge(vec3 StartPos, vec3 EndPoint, float width, float hight, float length);
	update_status Update(float dt) override;
	bool Draw() override;
	void AddPendulums();
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void ColorEffect(float dt);
	void CircuitPillarsEffect(float dt);

public:
	PhysBody3D * checkpoints[5] = { nullptr };

private:
	p2DynArray<Cube> cubeElems;
	p2DynArray<Cube> circuitPillars;
	p2DynArray<PhysBody3D*> bridges;
	Cube bridgeCube;

	p2DynArray<Cube> RampsCubes[8];
	p2DynArray<Cub> obstacles;

	Color highlightColor;

	//Color effect
	float currRampSegment = 0.f;
	Color currSegmentColor;
	p2DynArray<Cube> ramp[5];

	//Circuit pillar effect
	float pillarOffset = 0.f;
	float differenceBetweenPillars = 45.f;
	float transitionSpeed = 90.f;
	float baseHeight = 20.f;
	float additionalHeight = 10.f;
	Color pillarColor[2];

	Pendulum pen[7];

	Sphere sphere1;
	Sphere sphere2;

};
