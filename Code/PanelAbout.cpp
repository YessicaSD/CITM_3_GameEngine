#include "PanelAbout.h"
#include "imgui/imgui.h"
#include "Globals.h"
#include "Application.h"
PanelAbout::PanelAbout(const char* name, bool active): Panel(name, active)
{
}
void PanelAbout::Draw()
{
	
	ImGui::Text("Version %s", VERSION);
	ImGui::Separator();
	ImGui::Text("By Yessica Servin & Jaume Montagut i Guix");
	ImGui::Text("");
	ImGui::Text("Hinata Engine is licensed under the Public Domain,");
	ImGui::Text("click the button below  for more information.");
	ImGui::Text("");
	if (ImGui::Button("see more..."))
	{
		App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/blob/master/LICENSE.md");
	}
}
