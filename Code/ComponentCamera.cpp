#include "ComponentCamera.h"
#include "imgui/imgui.h"

ComponentCamera::ComponentCamera(GameObject* gameobject):Component(gameobject)
{
	name = "Camera";
}

void ComponentCamera::OnPostUpdate()
{

}

void ComponentCamera::PropertiesEditor()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{

	}
}
