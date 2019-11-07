#include "ComponentCamera.h"
#include "imgui/imgui.h"
#include "GameObject.h"

#include "glew/include/GL/glew.h"
ComponentCamera::ComponentCamera(GameObject* gameobject):Component(gameobject)
{
	name = "Camera";
	far_ = 10;
}

void ComponentCamera::OnPostUpdate()
{
	float3 z = gameobject->transform->GetZAxis();
	float3 position = gameobject->transform->GetPosition();

	float length = far_;
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);  // Blue
	glVertex3f(position.x, position.y, position.z);
	glVertex3f(position.x + z.x * length, position.y + z.y * length, position.z + z.z * length);
	glEnd();

}

void ComponentCamera::PropertiesEditor()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{

	}
}
