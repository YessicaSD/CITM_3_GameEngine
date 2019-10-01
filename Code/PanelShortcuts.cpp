#include "PanelShortcuts.h"

#include "Application.h"
#include "ModuleGui.h"
#include "Shortcut.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"

PanelShortcuts::PanelShortcuts(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) : Panel("Panel " + name, active, shortcuts)
{
}

void PanelShortcuts::Draw()
{
	float keys_column_distance = 175;
	float button_column_distance = 275;

	//TODO: Make the window have a minimum size that allows you to see everything (name + key combination + reset button)
	ImGui::Begin(name.c_str());

	ImGui::Text("Tool");
	ImGui::SameLine(keys_column_distance);
	ImGui::Text("Shortcut");
	ImGui::SameLine(button_column_distance);
	ImGui::Text("Reset shortcut");

	//TODO: Add a line to separate the titles for each column from the contents of it

	for (std::vector<Shortcut*>::iterator iter = App->gui->shortcuts.begin();
		iter != App->gui->shortcuts.end();
		++iter)
	{
		ImGui::Text((*iter)->name.c_str());
		ImGui::SameLine(keys_column_distance);
		ImGui::Text(ScancodeToString((*iter)->keys).c_str());
		ImGui::SameLine(button_column_distance);
		if (ImGui::Button("Reset"))
		{
			modifying_shortcut = true;
			shortcut_to_modify = (*iter);
		}
		//When the button is pressed, another panel appears which shows you the keys you've pressed
		//You can exit that panel by pressing esc
	}
	ImGui::End();

	if (modifying_shortcut)
	{
		ShowModifyShortcutPanel();
	}
}

//TODO: Make this new panel appear in front of the modifify shortcuts panel and with focus
std::string PanelShortcuts::ScancodeToString(std::vector<SDL_Scancode> keys)
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

//This is not a panel "per se"
//It's just a helper
void PanelShortcuts::ShowModifyShortcutPanel()
{
	//TODO: Show if there is already a shortcut with the same key combination
	ImGui::Begin("Reset shortcut");
	ImGui::Text("Press the desired key combination");
	ImGui::Text(ScancodeToString(new_key_combination).c_str());
	if (ImGui::Button("Cancel"))
	{
		modifying_shortcut = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		modifying_shortcut = false;
		//TODO: Assign new key combination
		//shortcut_to_modify->keys = new_key_combination;
	}
	ImGui::End();
}