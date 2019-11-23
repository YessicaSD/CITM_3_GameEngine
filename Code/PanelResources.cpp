#include "PanelResources.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleResourceManager.h"

PanelResources::PanelResources(std::string name, bool state, std::vector<SDL_Scancode> shortcuts) : Panel(name, state, shortcuts)
{
}

void PanelResources::Draw()
{
	ImGui::Begin(name.c_str());
	int space = 300;
	for (auto iter = App->resource_manager->resources.begin(); iter != App->resource_manager->resources.end(); ++iter)
	{
		char buffer[UID_DIGITS];
		sprintf_s(buffer, "%020llu", (*iter).first);
		ImGui::Selectable(buffer, false);
		ImGui::SameLine(space);
		std::string count = std::to_string((*iter).second->GetReferenceCount());
		ImGui::Text(count.c_str());
		//TODO: Add preview when you click it
		//TODO: Add type
	}
	ImGui::End();
}
