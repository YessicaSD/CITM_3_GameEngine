#include "PanelAbout.h"
#include "imgui/imgui.h"
#include "Globals.h"
#include "Application.h"
PanelAbout::PanelAbout(std::string name, bool active, std::vector<SDL_Scancode> shortcuts):
	Panel(name, active, shortcuts)
{
}
void PanelAbout::Draw()
{
	ImGui::Begin(name.c_str());
	ImGui::Text("Version %s", VERSION);
	ImGui::Separator();
	ImGui::Text("By Yessica Servin & Jaume Montagut i Guix");
	ImGui::Text("");
	ImGui::Text("Hinata Engine is licensed under the Public Domain,");
	ImGui::Text("click the button below  for more information.");
	ImGui::Text("");
	if (ImGui::Button("see more ..."))
	{
		App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/blob/master/LICENSE.md");
	}

	if(ImGui::CollapsingHeader("Credits"))
	{
		ImGui::Text("Hinata Engine Website:");
		if(ImGui::Button("More"))
			App->RequestBrowser("https://yessicasd.github.io/CITM_3_GameEngine/");

		ImGui::Text("Yessica Servin Dominguez:");
		if(ImGui::Button("More"))
			App->RequestBrowser("https://github.com/YessicaSD");

		ImGui::Text("Jaume Montagut i Guix:");
		if(ImGui::Button("More"))
			App->RequestBrowser("https://github.com/JaumeMontagut");
	}
	ImGui::End();
}
