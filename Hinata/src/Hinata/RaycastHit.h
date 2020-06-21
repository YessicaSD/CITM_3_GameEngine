#ifndef RAYCAST_HIT_H_
#define RAYCAST_HIT_H_
#include "ComponentTransform.h"
#include "../MathGeoLib/include/Math/float3.h"

struct RaycastHit
{
public:
	ComponentTransform* transform;
	float3 hit_point;
	float distance;

	RaycastHit() : transform(nullptr), hit_point(float3::zero), distance(0)
	{};
	RaycastHit(ComponentTransform* transform) : transform(transform), hit_point(float3::zero), distance(0)
	{};
	
};
#endif // !RAYCAST_HIT_H_

