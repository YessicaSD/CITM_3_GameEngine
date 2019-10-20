#include "PanelProperties.h"
#include "ComponentTransform.h"
#include "GameObject.h"
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
	if (selected_gameobject)
	{
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::Text("%s", selected_gameobject->gameobject->GetName());

	}
}
