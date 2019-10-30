#include "PanelAssets.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Texture.h"
#include "ModuleGui.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

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
		Texture* texture = (*iter).second;
		if(ImGui::ImageButton((void*)(intptr_t)texture->buffer_id, ImVec2(image_size, image_size)))
		{
			ComponentTransform* selected_tranform = App->gui->GetSelecteTransform();
			if (selected_tranform)
			{
				GameObject*  selected_gameobject = selected_tranform->gameobject;
				ComponentMaterial* material  = selected_gameobject->GetComponent<ComponentMaterial>();
				if (material)
				{
					material->SetTexture(texture);
				}
			}
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
