#include "PanelAbout.h"
#include "glew/include/GL/glew.h"
#include "imgui/imgui.h"
#include "Globals.h"
#include "Application.h"
#include "Assimp/include/version.h"
#include "DevIL/include/il.h"

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

		ImGui::Separator();
	}
	if (ImGui::CollapsingHeader("Libreries"))
	{
		SDL_version sdl_version;
		SDL_GetVersion(&sdl_version);
		ImGui::Text("SDL %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);
		ImGui::Text("OpenGL %s", glGetString(GL_VERSION));
		ImGui::Text("Glew %s", glewGetString(GLEW_VERSION));
		ImGui::Text("ImGui %s", ImGui::GetVersion());
		ImGui::Text("DevIL: %i", ilGetInteger(IL_VERSION_NUM));
		ImGui::Text("PCG Random Number Generator: 0.98");
		ImGui::Text("MathGeoLib");
		ImGui::Text("Assimp: %i.%i.%i ", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
		ImGui::Text("Bass: 2.4");
		ImGui::Text("Par Shapes");
		
	}
	ImGui::End();
}
