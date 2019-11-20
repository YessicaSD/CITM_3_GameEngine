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
	
	aspect_ratio = 1.3f;
	v_fov = frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * aspect_ratio);
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
	ImGui::Separator();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		bool changed_near_plane, changed_far_plane, changed_field_of_view, changed_aspect_ratio;
		
		
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		ImGui::Checkbox("Active", &frustum_culling);
		
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		if (changed_near_plane = ImGui::InputFloat("Near Plane", &near_plane, 1, 5, 2))
		{
			SetNearPlane(near_plane);
		}
		
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		if (changed_far_plane = ImGui::InputFloat("Far Plane", &far_plane, 1, 5, 2))
		{
			SetFarPlane(far_plane);
		}

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		if (changed_field_of_view = ImGui::SliderAngle("Field of view", &v_fov, 0.0f, 179.0f))
		{
			SetFieldOfView(v_fov);
		}

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		if (changed_aspect_ratio = ImGui::SliderFloat("Aspect_ratio", &aspect_ratio, 0, 5))
		{
			SetAspectRatio(aspect_ratio);
		}
		if(changed_near_plane || changed_far_plane || changed_field_of_view || changed_aspect_ratio)
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

void ComponentCamera::LookAt(float3 point)
{
	float3 vector = point - frustum.pos;
	Quat quat = Quat::LookAt(frustum.front, vector.Normalized(), frustum.up, float3::unitY);
	frustum.front = quat.Mul(frustum.front.Normalized());
	frustum.up = quat.Mul(frustum.up.Normalized());
}

bool ComponentCamera::IsInFrustum(const AABB & bounding_box)
{
	float3 corners[8];
	bounding_box.GetCornerPoints((float3*)&corners);
	return IsInFrustum(corners);
}

bool ComponentCamera::IsInFrustum(const OBB & bounding_box)
{
	float3 corners[8];
	bounding_box.GetCornerPoints((float3*)&corners);

	return IsInFrustum(corners);
}

bool ComponentCamera::IsInFrustum(math::float3  corners[8])
{
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

void ComponentCamera::SetFieldOfView(float & angle)
{
	 frustum.verticalFov = angle;
	 frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * aspect_ratio);
	 update_project_matrix = true;
}

void ComponentCamera::SetAspectRatio(float & ratio)
{
	aspect_ratio = ratio;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * aspect_ratio);
}

void ComponentCamera::SetPos(float3 & new_pos)
{
	frustum.pos = new_pos;
}

float4x4 ComponentCamera::GetViewMatrix()
{
	float4x4 ret = frustum.ViewMatrix();
	return ret.Transposed();
}

float4x4 ComponentCamera::GetProjectionMatrix()
{
	return frustum.ProjectionMatrix();
}

Frustum ComponentCamera::GetFrustrum()
{
	return frustum;
}





