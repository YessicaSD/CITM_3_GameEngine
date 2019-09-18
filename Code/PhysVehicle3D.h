#pragma once

#include "PhysBody3D.h"
#include "glmath.h"
#include "Color.h"

class btRaycastVehicle;
class btVector3;
struct PhysBody3D;


struct Wheel
{
	vec3 connection; // origin of the ray. Must come from within the chassis
	vec3 direction; 
	vec3 axis;
	float suspensionRestLength; // max length for suspension in meters
	float radius;
	float width;
	bool front; // is front wheel ?
	bool drive; // does this wheel received engine power ?
	bool brake; // does breakes affect this wheel ?
	bool steering; // does this wheel turns ?
	bool hide = false;
	Color color=Blue;
};
struct  Chassis
{
	vec3 size;
	vec3 offset;
	Color color = White;
};


struct VehicleInfo
{
	~VehicleInfo();
	Chassis chassis;

	float mass;
	float suspensionStiffness; // default to 5.88 / 10.0 offroad / 50.0 sports car / 200.0 F1 car
	float suspensionCompression; // default to 0.83
	float suspensionDamping; // default to 0.88 / 0..1 0 bounces / 1 rigid / recommended 0.1...0.3
	float maxSuspensionTravelCm; // default to 500 cm suspension can be compressed
	float frictionSlip; // defaults to 10.5 / friction with the ground. 0.8 should be good but high values feels better (kart 1000.0)
	float maxSuspensionForce; // defaults to 6000 / max force to the chassis

	Wheel* wheels;
	uint numChassisParts;
	Chassis* chassisPart;
	int num_wheels;
	
};


struct PhysVehicle3D : public PhysBody3D
{
public:
	PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info);
	~PhysVehicle3D();

	void Render();
	void ApplyEngineForce(float force);
	void Brake(float force);
	void Turn(float degrees);
	float GetKmh() const;
	void ApplyTorque(const btVector3 &torque);
public:

	VehicleInfo info;
	btRaycastVehicle* vehicle;
	vec3 GetForwardVec();
	
	void SetRotation(const float& yaw, const float& pitch, const float& roll);
	void SetRotation(const btQuaternion rotQuat);
};