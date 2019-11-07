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
	float3 z = gameobject->transform->GetZAxis();
	float3 position = gameobject->transform->GetPosition();

	//float length = far_;
	//glLineWidth(5);
	//glBegin(GL_LINES);
	//glColor3f(1.0f, 1.0f, 1.0f);  // Blue
	//glVertex3f(position.x, position.y, position.z);
	//glVertex3f(position.x + z.x * length, position.y + z.y * length, position.z + z.z * length);
	//glEnd();
	frustum_render.Draw();

}

void ComponentCamera::PropertiesEditor()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{

	}
}

