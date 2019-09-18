#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"


ModulePlayer::ModulePlayer(Application* app, int playerIdx, bool start_enabled) : playerIdx(playerIdx), Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}


ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	float chassisHeight = 1.f;

	// Car properties ----------------------------------------
	car.chassis.size.Set(2, 1, 3);
	car.chassis.offset.Set(0, chassisHeight, -1);
	car.chassis.color = color_Chassis;
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;


	// Don't change anything below this line ------------------
	float half_width = car.chassis.size.x*0.5f;
	float half_length = car.chassis.size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	//Chassis Parts ------------------------------------------
		car.numChassisParts = 8;
		car.chassisPart = new Chassis[car.numChassisParts];

		
		//BackPart -----------------------------------------------
		vec3 BackPartSize = { 1, 1, 7 };
		car.chassisPart[0].size = BackPartSize;
		car.chassisPart[0].offset.Set(1.f, chassisHeight, -3);
		car.chassisPart[0].color = color_Chassis;

		car.chassisPart[1].size = BackPartSize;
		car.chassisPart[1].offset.Set(-1.f, chassisHeight,-3 );
		car.chassisPart[1].color = color_Chassis;

		vec3 BackPart2Size = { 1.25f, 1.f, 6.0f };
		car.chassisPart[4].size = BackPart2Size;
		car.chassisPart[4].offset.Set(-1.5f, chassisHeight, -7);
		car.chassisPart[4].color = color_Chassis;

		car.chassisPart[5].size = BackPart2Size;
		car.chassisPart[5].offset.Set(1.5f, chassisHeight, -7);
		car.chassisPart[5].color = color_Chassis;

		//FrontPart ----------------------------------------------
		vec3 frontPartSize = { 0.5, 0.5, 2 };
		car.chassisPart[2].size = frontPartSize;
		car.chassisPart[2].offset.Set(0.8f, chassisHeight, half_length);
		car.chassisPart[2].color = color_Chassis;

		car.chassisPart[3].size = frontPartSize;
		car.chassisPart[3].offset.Set(-0.8f, chassisHeight, half_length);
		car.chassisPart[3].color = color_Chassis;

		car.chassisPart[6].size = { 1,0.6f,2 };
		car.chassisPart[6].offset.Set(0, chassisHeight+ car.chassisPart[6].size.y, -1);
		car.chassisPart[6].color.Set(color_Chassis.r,color_Chassis.g-0.5,color_Chassis.b-0.5);

		//Light --------------------------------
		car.chassisPart[7].size = {0.75,0.5,0.5};
		car.chassisPart[7].offset.Set(0, chassisHeight + car.chassisPart[7].size.y, 0);
		car.chassisPart[7].color = {1,1,139/255};

		

		// Wheel properties ---------------------------------------
		float connection_height = 2.f;
		float wheel_radius = 0.6f;
		float wheel_width = 0.8f;
		float suspensionRestLength = 1.2f;

		car.num_wheels = 4;
		car.wheels = new Wheel[car.num_wheels];

	// FRONT ------------------------
	car.wheels[0].connection.Set(0, connection_height, half_length + wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius*1.7f;
	car.wheels[0].width = 1;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;
	car.wheels[0].color = color_wheel;

	// REAR-LEFT ------------------------
	car.wheels[1].hide = true;
	car.wheels[1].connection.Set(half_width + wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius*2;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = false;
	car.wheels[1].drive = false;
	car.wheels[1].brake = true;
	car.wheels[1].steering = false;
	car.wheels[1].color = color_wheel;

	// REAR-RIGHT ------------------------
	car.wheels[2].hide = true;
	car.wheels[2].connection.Set(-half_width - wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius*2;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;
	car.wheels[2].color = color_wheel;

	// REAR-CENTER-BACK -----------------------
	car.wheels[3].connection.Set(0, connection_height, -half_length - wheel_radius*5);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius * 3;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;
	car.wheels[3].color = color_wheel;
	vehicle = App->physics->AddVehicle(car);

	vehicle->SetPos(324.f + 10.f * playerIdx, 0.f, 392.f);
	vehicle->SetRotation(M_PI, 0.f, 0.f);
	

	victorySFX = App->audio->LoadFx("Assets/SFX/victory.ogg");
	

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

update_status ModulePlayer::PreUpdate(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		debugPlayer = !debugPlayer;
	}

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::Update(float dt)
{
	if (!debugPlayer) {
		Movement();
	}
	else {
		DebugMovement();
	}

	Respawn();

	return UPDATE_CONTINUE;
}

void ModulePlayer::Respawn()
{
	//Start holding
	if (App->input->GetMouseButton(playerIdx, MOUSE_LEFT) == KEY_REPEAT && App->input->GetMouseButton(playerIdx, MOUSE_RIGHT) == KEY_REPEAT && !respawnTimerStarted) {
		respawnTimer.Start();
		respawnTimerStarted = true;
	}

	//Not holding for 1s
	if (App->input->GetMouseButton(playerIdx, MOUSE_LEFT) != KEY_REPEAT && App->input->GetMouseButton(playerIdx, MOUSE_RIGHT) != KEY_REPEAT) {
		respawnTimerStarted = false;
	}

	if (respawnTimer.Read() >= respawnClickTime && respawnTimerStarted) {
		//Respawn
		vec3 respawnPos = lastCheckpoint->GetPos();
		vehicle->SetPos(respawnPos.x, respawnPos.y, respawnPos.z);
		vehicle->SetRotation(lastCheckpoint->GetRotation());
		respawnTimerStarted = false;
	}
}

void ModulePlayer::DebugMovement()
{
	turn = acceleration = brake = 0.0f;
	speed = vehicle->GetKmh();

	vec3 forward_vector = vehicle->GetForwardVec();

	forward_vector = -forward_vector;
	forward_vector = forward_vector * 10;
	forward_vector.y = forward_vector.y + 5;


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (speed < MAX_VELOCITY) {
			acceleration = MAX_ACCELERATION;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES) {
			turn -= TURN_DEGREES;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES) {
			turn += TURN_DEGREES;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{

		if (speed > -MAX_VELOCITY) {
			acceleration = -MAX_ACCELERATION;
		}
	}


	vec3 up_pos = { vehicle->GetPos().x, vehicle->GetPos().y + 5, vehicle->GetPos().z };


	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
}

bool ModulePlayer::Draw() {
	vehicle->Render();
	return true;
}

vec3 ModulePlayer::GetPos()
{
	return vehicle->GetPos();
}

vec3 ModulePlayer::GetForwardVec()
{
	
	return vehicle->GetForwardVec();
}

void ModulePlayer::Movement()
{
	turn = acceleration = brake = 0.0f;
	vec3 torqueDir = vehicle->GetForwardVec() * 3250.0f;
	btVector3 torque(torqueDir.x, torqueDir.y, torqueDir.z);

	//Turn right
	if (App->input->GetMouseButton(playerIdx, MOUSE_LEFT) == KEY_DOWN ||
		App->input->GetMouseButton(playerIdx, MOUSE_LEFT) == KEY_REPEAT)
	{
		vehicle->ApplyTorque(torque);
		if (turn > -turnDegrees)
			turn -= turnDegrees;
	}

	//Turn left
	if (App->input->GetMouseButton(playerIdx, MOUSE_RIGHT) == KEY_DOWN ||
		App->input->GetMouseButton(playerIdx, MOUSE_RIGHT) == KEY_REPEAT)
	{
		vehicle->ApplyTorque(-torque);
		if (turn < turnDegrees)
			turn += turnDegrees;
	}

	//Boost the car
	if (App->input->GetScrollWheelState(playerIdx) == SCROLL_BACKWARD)
	{
		vec3 vec = normalize(vehicle->GetForwardVec()) * boostPower;
		vehicle->Push(vec.x, vec.y, vec.z);
	}

	//Move the car automatically
	if (vehicle->GetKmh() < defaultSpeed) {
		acceleration += catchAcceleration;
	}

	//Slow the car the faster is going
	else if (vehicle->GetKmh() > defaultSpeed && !(App->input->GetScrollWheelState(playerIdx) == SCROLL_BACKWARD)) {
		acceleration -= baseDeacceleration + multiplierDeacceleration * (vehicle->GetKmh() - defaultSpeed);
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

}