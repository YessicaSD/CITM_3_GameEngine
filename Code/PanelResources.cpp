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
	int space_1 = 200;
	int space_2 = 300;
	
	ImGui::Text("Resource UID");
	ImGui::SameLine(space_1);
	ImGui::Text("Type");
	ImGui::SameLine(space_2);
	ImGui::Text("Reference count");

	//TODO: Add file from which it was created

	ImGui::Separator();

	for (auto iter = App->resource_manager->resources.begin(); iter != App->resource_manager->resources.end(); ++iter)
	{
		char buffer[UID_DIGITS];
		sprintf_s(buffer, "%020llu", (*iter).first);
		ImGui::Selectable(buffer, false);
		ImGui::SameLine(space_1);
		ImGui::Text("%i", (*iter).second->type);
		ImGui::SameLine(space_2);
		std::string count = std::to_string((*iter).second->GetReferenceCount());
		ImGui::Text(count.c_str());
		//TODO: Add preview when you click it
		//TODO: Add type
	}
	ImGui::End();
}
