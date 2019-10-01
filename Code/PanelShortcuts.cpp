#include "PanelShortcuts.h"

#include "Application.h"
#include "ModuleGui.h"
#include "Shortcut.h"
#include "imgui/imgui.h"

PanelShortcuts::PanelShortcuts(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) : Panel("Panel " + name, active, shortcuts)
{
}

void PanelShortcuts::Draw()
{
	float second_column_distance = 200;

	ImGui::Begin("Shortcuts");
	for (std::vector<Shortcut*>::iterator iter = App->gui->shortcuts.begin();
		iter != App->gui->shortcuts.end();
		++iter)
	{
		ImGui::Text((*iter)->name.c_str());
		ImGui::SameLine(second_column_distance);
		ImGui::Text(GetShortcutValue((*iter)->keys).c_str());
	}
	ImGui::End();
}

std::string PanelShortcuts::GetShortcutValue(std::vector<SDL_Scancode> keys)
{
	std::string ret_string;
	for (std::vector<SDL_Scancode>::iterator iter = keys.begin();
		iter != keys.end();
		++iter)
	{
		ret_string = ret_string + std::to_string((*iter)) + " ";
	}
	return ret_string;
}
