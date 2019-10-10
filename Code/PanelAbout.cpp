#include "PanelAbout.h"
#include "imgui/imgui.h"
PanelAbout::PanelAbout(const char* name, bool active): Panel(name, active)
{
}
void PanelAbout::Draw()
{
	
	ImGui::Text("hola");
}
