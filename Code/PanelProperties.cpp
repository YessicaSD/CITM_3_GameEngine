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
		if (ImGui::Checkbox("", &selected_transform->gameobject->active))
		{
			LOG("Active checkbox");
			//TODO: Call SetActive with the corresponding value
		}
		ImGui::SameLine();
		//const int name_buffer_size = 64;
		//char name_buffer[name_buffer_size] = "";
		//ImGui::InputText(selected_transform->gameobject->GetName(), name_buffer, name_buffer_size);//finish
		ImGui::InputText("", &selected_transform->gameobject->name);

		//todo add enable / disable for each component
		//maybe on a function
		//so not all of them have that capability
		for (std::vector<Component *>::iterator iter = selected_transform->gameobject->components.begin();
			iter != selected_transform->gameobject->components.end();
			++iter)
		{
			(*iter)->PropertiesEditor();
		}
	}
	ImGui::End();
}

const ComponentTransform * PanelProperties::GetSelecteTransform()
{
	return selected_transform;
}
