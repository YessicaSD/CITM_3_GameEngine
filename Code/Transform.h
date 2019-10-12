#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"

class GameObject;

class Transform : public Component
{
public:
	Transform(GameObject * gameobject);

public:
	float3 position;
	float3 rotation;

};
#endif // !TRANSFORM_H_

