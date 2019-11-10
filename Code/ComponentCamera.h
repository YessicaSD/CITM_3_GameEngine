#ifndef COMPONENT_CAMERA_H_
#define COMPONENT_CAMERA_H_

#include "Component.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "CubeLine.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* gameobject);
	void OnPostUpdate() override;
	void PropertiesEditor() override;
	void TransformHaveChanged() override;
	void UpdateDrawingRepresentation();
	bool frustum_culling = true;
	bool IsInFrustum(const AABB& bounding_box);
	bool IsInFrustum(const OBB& bounding_box);
	bool IsInFrustum(math::float3  corners[8]);
	void SetNearPlane(const float& value);
	void SetFarPlane(const float& value);

	
private:
	float near_plane;
	float far_plane;
	float v_fov;

	Frustum frustum;
	CubeLine frustum_render;
	float4 background_color;
};
#endif // !COMPONENT_CAMARA_H_

