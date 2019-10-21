#include "PanelProperties.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui/imgui.h"

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
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::Text("%s", selected_transform->gameobject->GetName());

		for (std::vector<Component *>::iterator iter = selected_transform->gameobject->components.begin();
			iter != selected_transform->gameobject->components.end();
			++iter)
		{
			(*iter)->ShowProperties();
		}
	}
	ImGui::End();
}
