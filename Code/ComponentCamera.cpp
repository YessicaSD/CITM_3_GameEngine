#include "ComponentCamera.h"

#include "imgui/imgui.h"
#include "GameObject.h"
#include "Globals.h"


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

