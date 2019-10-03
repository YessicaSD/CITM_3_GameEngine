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

	ImGui::Separator();

	for (std::vector<Shortcut*>::iterator iter = App->gui->shortcuts.begin();
		iter != App->gui->shortcuts.end();
		++iter)
	{
		ImGui::Text((*iter)->name.c_str());
		ImGui::SameLine(keys_column_distance);
		ImGui::Text(GetKeysCharPtr((*iter)->keys));
		ImGui::SameLine(button_column_distance);
		ImGui::PushID((*iter));//We'll use the number of the pointer to differentiate from other buttons
		if (ImGui::Button("Reset"))
		{
			if (!modifying_shortcut)
			{
				modifying_shortcut = true;
				shortcut_to_modify = (*iter);
			}
		}
		ImGui::PopID();
		//When the button is pressed, another panel appears which shows you the keys you've pressed
		//You can exit that panel by pressing esc

		//TODO: You shouldn't be able to focus the "Shortcuts panel" if you have the "Modfiy shortcuts panel" enabled
	}
	ImGui::End();

	if (modifying_shortcut)
	{
		ShowModifyShortcutPanel();
	}
}



//TODO: Make this new panel appear in front of the modifify shortcuts panel and with focus
//This is not a panel "per se"
//It's just a helper
void PanelShortcuts::ShowModifyShortcutPanel()
{
	//TODO: Show if there is already a shortcut with the same key combination
	ImGui::Begin("Reset shortcut");
	ImGui::Text("Press the desired key combination");
	ImGui::Text(GetKeysCharPtr(new_key_combination));
	if (ImGui::Button("Cancel"))
	{
		modifying_shortcut = false;
		new_key_combination.clear();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		modifying_shortcut = false;
		shortcut_to_modify->keys = new_key_combination;
		new_key_combination.clear();
	}
	ImGui::End();
}

const char * PanelShortcuts::GetKeysCharPtr(std::vector<SDL_Scancode> keys)
{
	const uint buffer_size = 128u;
	uint curr_buffer_size = 1u;//1 space for the /0 character at the end
	char buffer[buffer_size];
	for (int i = 0; i < keys.size(); ++i)
	{
		const char * new_key = SDL_GetScancodeName(keys[i]);
		uint new_key_size = strlen(new_key);
		if (curr_buffer_size + new_key_size <= buffer_size)
		{
			strcat(buffer, new_key);
		}
		else
		{
			LOG("Buffer overflow, please increase the buffer size.");
			break;
		}
	}
	//strcat(buffer, '\0');
	//TODO: We should add the null character at the end
	return buffer;
}

void PanelShortcuts::ModifyShortcut(SDL_Scancode key)
{
	if (modifying_shortcut)
	{
		new_key_combination.push_back(key);
	}
}