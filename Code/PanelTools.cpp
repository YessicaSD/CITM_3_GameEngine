#include "PanelTools.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "imgui/imgui.h"
#include "Texture.h"
PanelTools::PanelTools(std::string name, bool active, std::vector<SDL_Scancode> shortcuts):Panel(name, active, shortcuts)
{
	atlas = App->texture->LoadTexture("Atlas.png");
	button_width = 72.0f / (float)atlas->width;
	button_height = 72.0f / (float)atlas->height;
}
void PanelTools::Draw()
{
	ImGui::Begin(name.c_str());
	
	//PLAT BUTTON
	if (ImGui::ImageButton((ImTextureID)atlas->buffer_id, ImVec2(15, 15), ImVec2(0, 0), ImVec2(button_width, button_height)))
	{
		if (App->IsStop())
		{
			App->Play();
		}
		if (App->IsPlay())
		{
			App->Stop();
		}
	}
	ImGui::SameLine();
	//PAUSE BUTTON
	if (ImGui::ImageButton((ImTextureID)atlas->buffer_id, ImVec2(15, 15), ImVec2(button_width, 0), ImVec2(button_width*2, button_height)))
	{
		if (App->IsPlay)
		{
			App->Pause();
		}
		if (App->IsPause())
		{
			App->UnPause();
		}
	}
	ImGui::SameLine();
	//NEXT STEP BUTTON
	if (ImGui::ImageButton((ImTextureID)atlas->buffer_id, ImVec2(15, 15), ImVec2(button_width * 2, 0), ImVec2(button_width * 3, button_height)))
	{
	
	
	}
	ImGui::End();
}
