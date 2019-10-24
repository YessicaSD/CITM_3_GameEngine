#include "PanelAssets.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Texture.h"
#include "ModuleGui.h"
PanelAssets::PanelAssets(std::string name, bool state, std::vector<SDL_Scancode> shortcuts):Panel(name, state, shortcuts)
{
}
void PanelAssets::Draw()
{
	ImGui::Begin(name.c_str());
	float num_colum = ImGui::GetWindowWidth()/ image_size;
	uint i = 1;
	for (std::map<std::string, Texture*>::iterator iter = App->texture->textures.begin();
		iter!= App->texture->textures.end();
		++iter)
	{
		ImGui::Image((void*)(intptr_t)(*iter).second->buffer_id, ImVec2(image_size, image_size));
		bool is_clicked = ImGui::IsItemClicked(0);
		if (is_clicked)
		{
			GameObject*  selected_gameObject = App->gui->GetSelecteTransform()->gameobject;
		}
		
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
	ImGui::End();
}
