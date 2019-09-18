#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{

	Cylinder wheel;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].hide)
			continue;

		wheel.color = info.wheels[i].color;
		wheel.radius = info.wheels[i].radius;
		wheel.height = info.wheels[i].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}


	Cube chassis(info.chassis.size.x, info.chassis.size.y, info.chassis.size.z);
	chassis.color = info.chassis.color;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis.offset.x, info.chassis.offset.y, info.chassis.offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube part;
	for (uint i = 0; i < info.numChassisParts; ++i)
	{
		part.color = info.chassisPart[i].color;
		part.size = info.chassisPart[i].size;
		vehicle->getChassisWorldTransform().getOpenGLMatrix(&part.transform);
		
		btVector3 offset(info.chassisPart[i].offset.x, info.chassisPart[i].offset.y, info.chassisPart[i].offset.z);
		offset = offset.rotate(q.getAxis(), q.getAngle());
		part.transform.M[12] += offset.getX();
		part.transform.M[13] += offset.getY();
		part.transform.M[14] += offset.getZ();

		part.Render();
	}

	chassis.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

void PhysVehicle3D::ApplyTorque(const btVector3 &torque)
{
	body->applyTorque(torque);
}

vec3 PhysVehicle3D::GetForwardVec()
{
	btVector3 aux = vehicle->getForwardVector();
	return {aux.x(), aux.y(), aux.z()};
}

void PhysVehicle3D::SetRotation(const btQuaternion rotQuat) {
	body->getWorldTransform().setRotation(rotQuat);
}

void PhysVehicle3D::SetRotation(const float& yaw, const float& pitch, const float& roll)
{
	btQuaternion rotQuat(yaw, pitch, roll);
	body->getWorldTransform().setRotation(rotQuat);
}
