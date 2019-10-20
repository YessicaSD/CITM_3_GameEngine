#include "PanelAssets.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Texture.h"
PanelAssets::PanelAssets(const char* name, bool state):Panel(name, state)
{
}
void PanelAssets::Draw()
{
	
	float num_colum = ImGui::GetWindowWidth()/ image_size;
	uint i = 1;
	for (std::map<std::string, Texture*>::iterator iter = App->texture->textures.begin();
		iter!= App->texture->textures.end();
		++iter)
	{
		ImGui::Image((void*)(intptr_t)(*iter).second->buffer_id, ImVec2(image_size, image_size));
		if (i == (uint)num_colum)
		{
			i = 1;
		}
		else
		{
			ImGui::SameLine();
			++i;

		}


	}
	//ImGui::Begin(name.c_str());
	//ImGui::End();
}
