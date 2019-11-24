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
	int space_1 = 175;
	int space_2 = 375;
	int space_3 = 475;
	
	ImGui::Text("Resource UID");
	ImGui::SameLine(space_1);
	ImGui::Text("Source");
	ImGui::SameLine(space_2);
	ImGui::Text("Type");
	ImGui::SameLine(space_3);
	ImGui::Text("Reference count");

	ImGui::Separator();

	for (auto iter = App->resource_manager->resources.begin(); iter != App->resource_manager->resources.end(); ++iter)
	{
		char buffer[UID_DIGITS];
		sprintf_s(buffer, "%020llu", (*iter).first);
		ImGui::Selectable(buffer, false);
		ImGui::SameLine(space_1);
		ImGui::Text((*iter).second->asset_source.c_str());
		ImGui::SameLine(space_2);
		ImGui::Text(App->resource_manager->GetResourceTypeString((*iter).second->GetType()));
		ImGui::SameLine(space_3);
		std::string count = std::to_string((*iter).second->GetReferenceCount());
		ImGui::Text(count.c_str());
		//TODO: Add preview when you click it
	}
	ImGui::End();
}
