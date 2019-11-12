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
	bool frustum_culling = true;

	ComponentCamera(GameObject* gameobject);
	void OnPostUpdate() override;
	void PropertiesEditor() override;
	void TransformHaveChanged() override;
	void UpdateDrawingRepresentation();
	void LookAt(float3 point);
	
	bool IsInFrustum(const AABB& bounding_box);
	bool IsInFrustum(const OBB& bounding_box);
	bool IsInFrustum(math::float3  corners[8]);


	void SetNearPlane(const float& value);
	void SetFarPlane(const float& value);
	void SetFieldOfView(float& angle);
	void SetAspectRatio(float& ratio);
	void SetPos(float3& new_pos);

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();

private:
	float near_plane;
	float far_plane;
	float v_fov;
	float aspect_ratio;
	Frustum frustum;
	CubeLine frustum_render;
	float4 background_color;

	friend class ModuleCamera3D;
};
#endif // !COMPONENT_CAMARA_H_

