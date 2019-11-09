#ifndef COMPONENT_CAMERA_H_
#define COMPONENT_CAMERA_H_

#include "Component.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "CubeLine.h"
#include "MathGeoLib/include/Geometry/AABB.h"
class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* gameobject);
	void OnPostUpdate() override;
	void PropertiesEditor() override;
	void TransformHaveChanged() override;
	bool frustum_culling = true;
	bool IsInfrustrum(const AABB& bounding_box);

private:

	Frustum frustum;
	CubeLine frustum_render;
	float4 background_color;
};
#endif // !COMPONENT_CAMARA_H_
