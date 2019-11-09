#include "ComponentCamera.h"

#include "imgui/imgui.h"
#include "GameObject.h"
#include "Globals.h"
#include "MathGeoLib/include/Geometry/Plane.h"


ComponentCamera::ComponentCamera(GameObject* gameobject):Component(gameobject)
{
	name = "Camera";
	frustum.type = math::FrustumType::PerspectiveFrustum;

	frustum.pos = math::float3::zero;
	frustum.front = math::float3::unitZ;
	frustum.up = math::float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 500.0f;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);
	float3 corners[8];
	frustum.GetCornerPoints(corners);
	frustum_render.SetVetices((float*)&corners);
}

void ComponentCamera::OnPostUpdate()
{
	frustum_render.Draw();
}

void ComponentCamera::PropertiesEditor()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{

	}
}

void ComponentCamera::TransformHaveChanged()
{
	frustum.pos = gameobject->transform->GetPosition();
	frustum.front = gameobject->transform->GetZAxis();
	frustum.up = gameobject->transform->GetYAxis();

	float3 corners[8];
	frustum.GetCornerPoints(corners);
	frustum_render.SetVetices((float*)&corners);
}

bool ComponentCamera::IsInfrustrum(const AABB & bounding_box)
{
	float3 corners[8];
	bounding_box.GetCornerPoints((float3*)&corners);

	for (uint index_plane = 0u; index_plane < 6; ++index_plane)
	{
		Plane plane = frustum.GetPlane(index_plane);
		int iInCount = 8;
		for (uint i = 0u; i < 8; i++)
		{
			if (plane.IsOnPositiveSide(corners[i]))
				--iInCount;
		}

		if (iInCount == 0)
			return false;
	}
	return true;
}





