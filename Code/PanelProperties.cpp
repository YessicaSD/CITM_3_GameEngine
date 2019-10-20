#include "PanelProperties.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui/imgui.h"

PanelProperties::PanelProperties(std::string name, bool state, std::vector<SDL_Scancode> shortcuts): Panel(name, state, shortcuts)
{
}
void PanelProperties::SetGameObject(ComponentTransform * gameobject)
{
	if (gameobject)
	{
		this->selected_gameobject = gameobject;
	}
}
void PanelProperties::Draw()
{
	ImGui::Begin(name.c_str());
	if (selected_gameobject != nullptr)
	{
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::Text("%s", selected_gameobject->gameobject->GetName());

		for (std::vector<Component *>::iterator iter = selected_gameobject->gameobject->components.begin();
			iter != selected_gameobject->gameobject->components.end();
			++iter)
		{
			(*iter)->ShowProperties();
		}
	}
	ImGui::End();
}
