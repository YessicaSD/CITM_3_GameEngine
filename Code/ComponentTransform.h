#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
class Transform : public Component
{
	public:
		float3 position;
		float3 rotation;

};
#endif // !TRANSFORM_H_

