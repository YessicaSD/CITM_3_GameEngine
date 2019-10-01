#include "PanelShortcuts.h"

#include "imgui/imgui.h"

PanelShortcuts::PanelShortcuts(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) : Panel(name, active, shortcuts)
{
}

void PanelShortcuts::Draw()
{
	ImGui::Begin("Shortcuts");
	ImGui::Text("Shortcuts panel");
	ImGui::End();
}
