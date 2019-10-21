#include "Component.h"
#include "imgui/imgui.h"
#include "Globals.h"

Component::Component(GameObject * gameobject):
	gameobject(gameobject)
{
}

void Component::EnableComponent(bool value)
{
	if (value)
	{
		enabled = true;
		OnEnable();
	}
	else
	{
		enabled = false;
		OnDisable();
	}
}

bool Component::CollapsigHeaderWithCheckbox()
{
	ImGui::PushID(name.c_str());//Because we don't want to display any label, the header can get confused with other headers without name and we need to push an id
	bool header_expanded = ImGui::CollapsingHeader("", ImGuiTreeNodeFlags_AllowItemOverlap);
	ImGui::PopID();
	ImGui::SameLine();
	LOG("%d", enabled);
	bool checkbox_pressed = ImGui::Checkbox(name.c_str(), &enabled);
	LOG("%d", enabled);
	//bool checkbox_pressed = ImGui::IsItemClicked();
	if (checkbox_pressed)
	{
		EnableComponent(!enabled);
	}
	return /*header_expanded &&*/ !checkbox_pressed;
}