#include "PanelProperties.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "Globals.h"

PanelProperties::PanelProperties(std::string name, bool state, std::vector<SDL_Scancode> shortcuts): Panel(name, state, shortcuts)
{
}
void PanelProperties::SetSelectedTransform(ComponentTransform * transform)
{
	if (transform != nullptr)
	{
		this->selected_transform = transform;
	}
}
void PanelProperties::Draw()
{
	ImGui::Begin(name.c_str());
	if (selected_transform != nullptr)
	{
		ImGui::PushID(name.c_str());//It doesn't have a name which identifies it, so we must Push and Pop the ID
		if (ImGui::Checkbox("", &selected_transform->gameobject->active))
		{
			selected_transform->gameobject->SetActive(selected_transform->gameobject->active);
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::InputText("", &selected_transform->gameobject->name);

		for (std::vector<Component *>::iterator iter = selected_transform->gameobject->components.begin();
			iter != selected_transform->gameobject->components.end();
			++iter)
		{
			(*iter)->PropertiesEditor();
		}
	}
	ImGui::End();
}
