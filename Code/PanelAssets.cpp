#include "PanelAssets.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ResourceTexture.h"
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
	//TODO: Show the name of the files in the assets folder
	//If they are dragged, they are really dragging the custom formats
	ImGui::End();
}

//float num_colum = ImGui::GetWindowWidth() / image_size;
//uint i = 1;
//for (std::map<std::string, ResourceTexture*>::iterator iter = App->texture->textures.begin();
//	iter != App->texture->textures.end();
//	++iter)
//{
//	ResourceTexture* texture = (*iter).second;
//	if (ImGui::ImageButton((void*)(intptr_t)texture->buffer_id, ImVec2(image_size, image_size)))
//	{
//		ComponentTransform* selected_transform = App->gui->GetSelectedTransform();
//		if (selected_transform != nullptr)
//		{
//			GameObject*  selected_gameobject = selected_transform->gameobject;
//			ComponentMaterial* material = selected_gameobject->GetComponent<ComponentMaterial>();
//			if (material != nullptr)
//			{
//				material->SetTexture(texture);
//			}
//		}
//	}
//
//	if (i == (uint)num_colum)
//	{
//		i = 1;
//	}
//	else
//	{
//		ImGui::SameLine();
//		++i;
//
//	}
//}