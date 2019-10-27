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

	ImGui::Text("Credits");
	
	ImGui::Text("Hinata Engine Website:");

	if(ImGui::Button("More"))
		App->RequestBrowser("https://yessicasd.github.io/CITM_3_GameEngine/");

	ImGui::Separator();

	if(ImGui::Button("Yessica Servin Dominguez"))
		App->RequestBrowser("https://github.com/YessicaSD");

	if(ImGui::Button("Jaume Montagut i Guix"))
		App->RequestBrowser("https://github.com/JaumeMontagut");

	ImGui::Separator();
	
	if (ImGui::CollapsingHeader("Libraries"))
	{
		SDL_version sdl_version;
		SDL_GetVersion(&sdl_version);
		if (ImGui::Button("SDL"))
		{
			App->RequestBrowser("https://www.libsdl.org/");
		}
		ImGui::SameLine();
		ImGui::Text("%i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);
		
		
		if (ImGui::Button("OpenGL"))
		{
			App->RequestBrowser("https://www.opengl.org/");
		}
		ImGui::SameLine();
		ImGui::Text("%s", glGetString(GL_VERSION));

		if (ImGui::Button("Glew"))
			App->RequestBrowser("http://glew.sourceforge.net/");
		ImGui::SameLine();
		ImGui::Text("%s", glewGetString(GLEW_VERSION));

		if (ImGui::Button("ImGui"))
			App->RequestBrowser("https://github.com/ocornut/imgui");
		ImGui::SameLine();
		ImGui::Text("%s", ImGui::GetVersion());

		if (ImGui::Button("DevIL"))
			App->RequestBrowser("http://openil.sourceforge.net/");
		ImGui::SameLine();
		ImGui::Text("%i", ilGetInteger(IL_VERSION_NUM));
		
		if (ImGui::Button("PCG Random Number Generator"))
			App->RequestBrowser("http://www.pcg-random.org/");
		ImGui::SameLine();
		ImGui::Text("0.98");

		if (ImGui::Button("MathGeoLib"))
			App->RequestBrowser("https://github.com/juj/MathGeoLib");
	
		if (ImGui::Button("Assimp"))
			App->RequestBrowser("");
		ImGui::SameLine();
		ImGui::Text("%i.%i.%i ", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
		
		if (ImGui::Button("Bass"))
			App->RequestBrowser("https://www.un4seen.com/");
		ImGui::SameLine();
		ImGui::Text("2.4");

		
	}
	ImGui::End();
}
