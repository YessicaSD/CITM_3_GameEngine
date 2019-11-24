#include "PanelTools.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "imgui/imgui.h"
#include "ResourceTexture.h"
#include "ModuleResourceManager.h"
#include "ModuleTime.h"

PanelTools::PanelTools(std::string name, bool active, std::vector<SDL_Scancode> shortcuts):Panel(name, active, shortcuts)
{
	JSONFile atlas_meta;
	atlas_meta.LoadFile(std::string("Assets/Atlas.png") + "." + META_EXTENSION);
	UID atlas_uid = atlas_meta.LoadUID("resourceUID");
	atlas = (ResourceTexture*)App->resource_manager->GetResource(atlas_uid);
	atlas->StartUsingResource();

	button_width = 72.0f / (float)atlas->width;
	button_height = 72.0f / (float)atlas->height;
}
void PanelTools::Draw()
{
	if (ImGui::Begin(name.c_str()))
	{
		//PLAT BUTTON
		ImGui::PushID("moveHovered");
		
		if(ImGui::ImageButton((ImTextureID)atlas->buffer_id, ImVec2(15, 15), ImVec2(0, 0), ImVec2(button_width, button_height)))
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
	
		ImGui::PopID();

		ImGui::SameLine();
		//PAUSE BUTTON
		ImGui::PushID("PauseButton");
		if (ImGui::ImageButton((ImTextureID)atlas->buffer_id, ImVec2(15, 15), ImVec2(button_width, 0), ImVec2(button_width*2, button_height)))
		{
			if (App->IsPlay())
			{
				App->Pause();
			}
			if (App->IsPause())
			{
				App->UnPause();
			}
		}
		ImGui::PopID();

		ImGui::SameLine();
		//NEXT STEP BUTTON
		ImGui::PushID("NextStepButton");
		if (ImGui::ImageButton((ImTextureID)atlas->buffer_id, ImVec2(15, 15), ImVec2(button_width * 2, 0), ImVec2(button_width * 3, button_height)))
		{
		
		
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::Text("%f", App->time->GetTime());
		
	}
	ImGui::End();

}
