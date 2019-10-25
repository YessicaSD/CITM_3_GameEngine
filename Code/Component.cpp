#include "Component.h"
#include "imgui/imgui.h"
#include "Globals.h"

const std::size_t Component::type = std::hash<std::string>()(TO_STRING(Component));

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
	bool header_expanded = ImGui::CollapsingHeader("", ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_OpenOnArrow);
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::Checkbox(name.c_str(), &enabled);
	bool checkbox_pressed = ImGui::IsItemClicked();//INFO: We're using IsItemClicked because the Checkbox function is always returning us false
	if (checkbox_pressed)
	{
		EnableComponent(!enabled);
	}
	return header_expanded;
}