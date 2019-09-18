#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#define MAX_VELOCITY 10000
#define MAX_ACCELERATION 10000
#define TURN_DEGREES 40
#define	BRAKE_POWER 100

struct PhysVehicle3D;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, int playerIdx, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	void Respawn();
	void DebugMovement();
	bool CleanUp();
	bool Draw() override;

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	Color color_Chassis;
	Color color_wheel = { 1.f,1.f,1.f,1.f };;

	vec3 GetPos();
	vec3 GetForwardVec();

private:
	void Movement();

public:
	PhysVehicle3D* vehicle;
	
	float turn;
	float acceleration;
	float brake;

	bool debugPlayer = false;
	int playerIdx = -1;//Used to identify the different players
	float speed = 0.0F;
	float boostPower = 500.f;//The power with wich the player is pushed forward when the scrollwheel is moved
	float defaultSpeed = 120.0f;// The speed at which the player moves if no one is giving any boost
	float baseDeacceleration = 3000.0f;//The acceleration that is going to be decreased if the player is going over the default speed
	//Decrease the acceleration in conjunction with baseDeacceleration.
	//It is multiplied by every 1 km/h that the player is going over the default speed.
	//Useful so that the player doesn't reach ridiculous speeds
	float multiplierDeacceleration = 100.0f;
	float catchAcceleration = 2000.f;//The acceleration at which the player is going to climb back to the default speed
	float turnDegrees = 15.0f * DEGTORAD;//The angle that the car is going to turn

	//Camera
	float maxCameraSpeed = defaultSpeed + 30.0f;//The speed at which the camera considers to be most away from the player

	//Checkpoints
	PhysBody3D * lastCheckpoint = nullptr;

private:
	//Respawn
	bool respawnTimerStarted = false;
	Timer respawnTimer;
	float respawnClickTime = 1000.f;

	int laps = 0;
	uint victorySFX = 0;
};