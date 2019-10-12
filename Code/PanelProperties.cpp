#include "PanelProperties.h"
#include "imgui/imgui.h"
PanelProperties::PanelProperties(std::string name, bool state, std::vector<SDL_Scancode> shortcuts): Panel(name, state, shortcuts)
{
}
void PanelProperties::Draw()
{
	//if (ImGui::CollapsingHeader("", &active))
	//{

	//}
}
