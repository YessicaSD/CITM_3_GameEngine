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
	~ComponentCamera();
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
	Frustum  GetFrustrum();

	bool update_project_matrix = false;
private:
	float near_plane = 0.0f;
	float far_plane = 0.0f;
	float v_fov = 0.0f;
	float aspect_ratio = 0.0f;
	Frustum frustum;
	CubeLine frustum_render;
	float4 background_color= float4::zero;

	friend class ModuleCamera3D;
};
#endif // !COMPONENT_CAMARA_H_

