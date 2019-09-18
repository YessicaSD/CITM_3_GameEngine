#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "ModulePlayer.h"
#include "ModuleAudio.h"
#include "TrackOutline.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	highlightColor.Set(1.f, 1.f, 0.f, 1.f);
	currSegmentColor.Set(0.2f, 0.5f, 0.7f, 1.f);//The first segment will need the default color
	pillarColor[0].Set(0.f, 0.f, 1.f, 1.f);
	pillarColor[1].Set(0.f, 0.f, 0.5f, 1.f);

	AddGround();
	AddRamps();
	AddTrack(TrackScaled0, TRACK_POSITIONS_0);
	AddTrack(TrackScaled1, TRACK_POSITIONS_1);
	AddTrack(TrackScaled2, TRACK_POSITIONS_2);
	AddTrack(TrackScaled3, TRACK_POSITIONS_3);
	AddTrack(TrackScaled4, TRACK_POSITIONS_4);
	AddTrack(TrackScaled5, TRACK_POSITIONS_5);
	AddCubes();
	AddSpheres();
	AddStartingPoint();
	AddCheckpoints();
	AddPendulums();

	App->audio->PlayMusic("Assets/Music/WipEout_Omega_OST_Invaders_Must_Die.ogg");

	return ret;
}

void ModuleSceneIntro::AddCubes()
{
	Cub aux;
	aux.RenderMesh.color = Red;
	aux.RenderMesh.SetScale(10, 10, 10);
	aux.RenderMesh.SetRotation(45, { 0.25,0.25,0 });
	aux.RenderMesh.SetPos(400, 0, 200);
	App->physics->AddBody(aux.RenderMesh, 0);
	obstacles.PushBack(aux);
}

void ModuleSceneIntro::AddSpheres()
{
	sphere1.SetPos(638.f, 0.f, 258.f);
	sphere1.radius = 30.f;
	sphere1.color.Set(15.f / 255.f, 36.f / 255.f, 51.f / 255.f);
	App->physics->AddBody(sphere1, 0.f);

	sphere2.SetPos(646.f, 0.f, 386.f);
	sphere2.radius = 30.f;
	sphere2.color.Set(15.f / 255.f, 36.f / 255.f, 51.f / 255.f);
	App->physics->AddBody(sphere2, 0.f);
}

void ModuleSceneIntro::AddStartingPoint()
{
	Cube startingPoint(45.f, 7.5f, 3.f);
	startingPoint.SetPos(340.f, 12.f, 410.f);

	startingPoint.color.Set(1.f, 1.f, 0.f, 1.f);//Change currSegment color, it will be overriden anyway
	cubeElems.PushBack(startingPoint);
}

void ModuleSceneIntro::AddCheckpoints()
{
	Cube sensorCube(45, 40, 3);

	sensorCube.SetPos(340, 2.5, 410);
	//sensorCube.SetRotation(90 ,{0.f, 1.f, 0.f});
	checkpoints[0] = App->physics->AddBody(sensorCube, 0.f, this);

	sensorCube.SetPos(567, 2.5, 125);
	sensorCube.SetRotation(120, { 0.f, 1.f, 0.f });
	checkpoints[1] = App->physics->AddBody(sensorCube, 0.f, this);

	sensorCube.SetPos(616, 2.5, 553);
	sensorCube.SetRotation(90, { 0.f, 1.f, 0.f });
	checkpoints[2] = App->physics->AddBody(sensorCube, 0.f, this);

	sensorCube.SetPos(210, 2.5, 560);
	sensorCube.SetRotation(90, { 0.f, 1.f, 0.f });
	checkpoints[3] = App->physics->AddBody(sensorCube, 0.f, this);

	sensorCube.SetPos(342, 2.5, 725);
	sensorCube.SetRotation(0, { 0.f, 1.f, 0.f });
	checkpoints[4] = App->physics->AddBody(sensorCube, 0.f, this);

	
}

void ModuleSceneIntro::AddGround()
{
	Cube ground(10000, 0, 10000);
	ground.SetPos(0, 0, 0);

	ground.color.Set(currSegmentColor);//Change currSegment color, it will be overriden anyway
	cubeElems.PushBack(ground);
}


void ModuleSceneIntro::AddTrack(int * trackOutline, int maxPos)
{
	Cube * cube = nullptr;
	float distanceBetweenPillars = 5.0f;
	float accumulatedDist = 0.f;
	btVector3 currNode(0.f,0.f,0.f);
	btVector3 nextNode(0.f, 0.f, 0.f);;
	btVector3 currPillarPos(0.f, 0.f, 0.f);;
	btVector3 dir(0.f, 0.f, 0.f);
	float angle = 0;
	int color = 0;

	// Iterate through the nodes
	for (int i = 0; i < maxPos - 2; i += 2) {
		currNode.setValue(trackOutline[i + 0], 0.f, trackOutline[i + 1]);
		nextNode.setValue(trackOutline[i + 2], 0.f, trackOutline[i + 3]);
		dir = (nextNode - currNode).normalize();
		//Add walls (colliders)
		Cube wall (btDistance(nextNode, currNode), 500.f, 1.5f);
		wall.SetPos((nextNode.getX() + currNode.getX())/2.f, 0.f,(nextNode.getZ() + currNode.getZ())/2.f);
		wall.SetRotation(atan2(dir.getZ(), dir.getX()) * -RADTODEG, { 0.f, 1.f, 0.f });
		App->physics->AddBody(wall, 0.f);
		//Put pillars between the current node and the next node (visuals)
		currPillarPos = currNode;
		while (btDistance(currPillarPos, nextNode) >= (distanceBetweenPillars - accumulatedDist)) {
			currPillarPos += dir * (distanceBetweenPillars - accumulatedDist);
			cube = new Cube(1.5f, baseHeight, 1.5f);
			cube->SetPos(currPillarPos.getX(), currPillarPos.getY(), currPillarPos.getZ());
			if (color == 0) {
				color = 1;
			}
			else if (color == 1) {
				color = 0;
			}
			cube->color.Set(pillarColor[color]);
			//cube->SetRotation(180, { 0,1,0 });
			circuitPillars.PushBack(*cube);
			accumulatedDist = 0.f;
		}
		accumulatedDist += btDistance(currPillarPos, nextNode);
	}
}

void ModuleSceneIntro::AddRamps()
{
	/*Cube cube(100, 1, 10);
	cube.color.Set(0.5F, 0, 0.2F, 0.0F);
	cube.SetPos(0, 0, 200);
	cube.SetRotation(180, { 0,1,0 });
	ramp[0] = App->physics->AddRamp(cube, 5, 90);
	for (uint i = 0; i < ramp[0].Count(); ++i)
	{
		ramp[0].At(i)->color.Set(0.2 + i * 2 * 0.01F, 0.5 + i * 5 * 0.01F, 0.7 + i * 7 * 0.01F, 1);
		cubeElems.PushBack(*ramp[0].At(i));
	}

	Cube aux(100, 1, 10);
	aux.SetRotation(180, { 0,1,0 });
	aux.SetPos(0, 0, 50);
	ramp[1] = App->physics->AddRamp(aux, 25, 25);
	for (uint i = 0; i < ramp[1].Count(); ++i)
	{
		ramp[1][i].color.Set(0.2 + i * 2 * 0.01F, 0.5 + i * 5 * 0.01F, 0.7 + i * 7 * 0.01F, 1);
		cubeElems.PushBack(ramp[1][i]);
	}


	aux.SetRotation(0, { 0,1,0 });
	aux.SetPos(0, 0, 140);
	ramp[2] = App->physics->AddRamp(aux, 25, 40);
	for (uint i = 0; i < ramp[2].Count(); ++i)
	{
		ramp[2][i].color.Set(0.2 + i * 2 * 0.01F, 0.5 + i * 5 * 0.01F, 0.7 + i * 7 * 0.01F, 1);
		cubeElems.PushBack(ramp[2][i]);
	}*/

	Cube aux(200, 1, 20);
	aux.SetPos(610, -0.5f, 140);
	aux.color.Set(15.f/255.f, 36.f /255.f, 51.f /255.f);
	RampsCubes[0] = App->physics->AddRamp(aux, 20, 90);
	
	

	aux.size.Set(145, 1, 100);
	aux.SetPos(600, -0.5f, 150);
	aux.SetRotation(-30, { 0,1,0 });
	RampsCubes[1] = App->physics->AddRamp(aux, 30, 80);


	aux.size.Set(25, 1, 80);
	aux.SetPos(600, -0.5f, 535);
	aux.SetRotation(90, { 0,1,0 });
	RampsCubes[2] = App->physics->AddRamp(aux, 30, 30);

	aux.size.Set(25, 1, 80);
	aux.SetPos(420, -0.5f, 550);
	aux.SetRotation(90, { 0,1,0 });
	RampsCubes[3] = App->physics->AddRamp(aux, 30, 45);

	aux.size.Set(25, 1, 80);
	aux.SetPos(269, -0.5f, 566);
	aux.SetRotation(-90, { 0,1,0 });
	RampsCubes[3] = App->physics->AddRamp(aux, 30, 45);

	aux.size.Set(25, 2, 290);
	aux.SetPos(343, -2.f, 560);
	aux.SetRotation(-90, { 0,1,0 });
	RampsCubes[3] = App->physics->AddRamp(aux, 15, 360);

	aux.size.Set(150, 2, 100);
	aux.SetPos(100, -1.f, 580);
	aux.SetRotation(20, { 0,1,0 });
	RampsCubes[4] = App->physics->AddRamp(aux, 20, 90);

	aux.size.Set(200, 2, 50);
	aux.SetPos(220, -1.f, 760);
	aux.SetRotation(0, { 0,1,0 });
	RampsCubes[5] = App->physics->AddRamp(aux, 30, 50);

	aux.size.Set(200, 2, 50);
	aux.SetPos(220, -1.f, 800);
	aux.SetRotation(180, { 0,1,0 });
	RampsCubes[6] = App->physics->AddRamp(aux, 30, 50);

	for (uint j = 0; j < 8; ++j)
	{
		for (uint i = 0; i < RampsCubes[j].Count(); ++i)
		{
			cubeElems.PushBack(RampsCubes[j][i]);
			
		}
	}

}
//p2DynArray<PhysBody3D*>  ModuleSceneIntro::AddBridge(vec3 StartPos, vec3 EndPoint, float width, float hight, float length)
//{
//	vec3 direcction = EndPoint - StartPos;
//	float BridgeLenght = Length(direcction);
//	int numCub = BridgeLenght / length;
//	direcction = normalize(direcction);
//	vec3 rotationVec = cross(direcction, { 0,0,1 });
//	vec3 x;
//	float angle = acos((x.x*direcction.x) / 1 * Length(direcction));
//
//	vec3 position;
//	p2DynArray<PhysBody3D*> ret;
//	Cube aux(width, hight, length);
//	position = StartPos;
//	aux.SetPos(position.x, position.y, position.z);
//	ret.PushBack(App->physics->AddBody(aux,0));
//
//	for (uint i = 1; i < numCub-1; i++)
//	{
//		position = StartPos + direcction * length*i;
//		aux.SetPos(position.x, position.y, position.z);
//		ret.PushBack(App->physics->AddBody(aux));
//		
//	}
//	uint i = numCub - 1;
//	position = StartPos + direcction * length*i;
//	aux.SetPos(position.x, position.y, position.z);
//	ret.PushBack(App->physics->AddBody(aux,0));
//
//	for (uint i = 0; i < numCub-1; ++i)
//	{
//		App->physics->AddConstraintHinge(*ret[i], *ret[i + 1], direcction*length, vec3(0, 0, 0), { 0,1,0 }, { 0,1,0 });
//	}
//	
//	return ret;
//
//}
//p2DynArray<PhysBody3D*>  ModuleSceneIntro::AddBridge(vec3 StartPos, vec3 EndPoint, float width, float length, float hight)
//{
//	vec3 direcction = EndPoint - StartPos;
//	float BridgeLenght = Length(direcction);
//	int numCub = BridgeLenght / length;
//	direcction=normalize(direcction);
//	vec3 position;
//	p2DynArray<PhysBody3D*> ret;
//	Cube aux(width, hight, length);
//	position = StartPos;
//	aux.SetPos(position.x, position.y, position.z);
//	ret.PushBack(App->physics->AddBody(aux,0));
//
//	for (uint i = 1; i < numCub-1; i++)
//	{
//		position = StartPos + direcction * aux.size.x*i;
//		aux.SetPos(position.x, position.y, position.z);
//		ret.PushBack(App->physics->AddBody(aux));
//		
//	}
//	uint i = numCub - 1;
//	position = StartPos + direcction * aux.size.x*i;
//	aux.SetPos(position.x, position.y, position.z);
//	ret.PushBack(App->physics->AddBody(aux,0));
//
//	for (uint i = 0; i < numCub-1; ++i)
//	{
//		App->physics->AddConstraintHinge(*ret[i], *ret[i + 1], direcction*aux.size, vec3(0, 0, 0), { 0,1,0 }, { 0,1,0 });
//	}
//	
//	return ret;
//
//}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::Update(float dt) {
	
	return UPDATE_CONTINUE;
}




bool ModuleSceneIntro::Draw()
{
	
	return true;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

