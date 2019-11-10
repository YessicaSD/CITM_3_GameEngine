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

	near_plane = frustum.nearPlaneDistance = 0.1f;
	far_plane = frustum.farPlaneDistance = 500.0f;
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
		bool changed_near_plane, changed_far_plane, changed_field_of_view;
		
		ImGui::Checkbox("Active", &frustum_culling);
		ImGui::Text("Near Plane"); 
		ImGui::SameLine(); 
		if (changed_near_plane = ImGui::InputFloat("##Near Plane", &near_plane, 1, 5, 2))
		{
			SetNearPlane(near_plane);
		}

		ImGui::Text("Far Plane");
		ImGui::SameLine();
		if (changed_far_plane = ImGui::InputFloat("##Far Plane", &far_plane, 1, 5, 2))
		{
			SetFarPlane(far_plane);
		}

		ImGui::Text("Field of view (Vertical FOV)");
		ImGui::SameLine();
		ImGui::SliderAngle("##VFOV", &v_fov);

		if(changed_near_plane || changed_far_plane)
			UpdateDrawingRepresentation();

	}
}

void ComponentCamera::TransformHaveChanged()
{
	frustum.pos = gameobject->transform->GetPosition();
	frustum.front = gameobject->transform->GetZAxis();
	frustum.up = gameobject->transform->GetYAxis();

	UpdateDrawingRepresentation();
}

void ComponentCamera::UpdateDrawingRepresentation()
{
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

void ComponentCamera::SetNearPlane(const float & value)
{
	if (value > 0.00f)
	{
		frustum.nearPlaneDistance = (value < frustum.farPlaneDistance) ? value : frustum.nearPlaneDistance;
	}
	near_plane = frustum.nearPlaneDistance;
}

void ComponentCamera::SetFarPlane(const float & value)
{

	frustum.farPlaneDistance = (value > frustum.nearPlaneDistance ) ? value : frustum.farPlaneDistance;
	
	far_plane = frustum.farPlaneDistance;
}





