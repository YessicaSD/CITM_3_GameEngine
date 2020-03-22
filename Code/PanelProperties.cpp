#include "PanelProperties.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "PanelAssets.h"
#include "ModuleResourceManager.h"
#include "ResourceTexture.h"

PanelProperties::PanelProperties(std::string name, bool state, std::vector<SDL_Scancode> shortcuts): Panel(name, state, shortcuts)
{
}

void PanelProperties::Draw()
{
	
	ImGui::Begin(name.c_str());

	if (selected_transform != nullptr)
	{
		ImGui::PushID(name.c_str());//It doesn't have a name which identifies it, so we must Push and Pop the ID
		if (ImGui::Checkbox("", &selected_transform->gameobject->active))
		{
			selected_transform->gameobject->SetActive(selected_transform->gameobject->active);
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::InputText("", &selected_transform->gameobject->name);

		for (std::vector<Component *>::iterator iter = selected_transform->gameobject->components.begin();
			iter != selected_transform->gameobject->components.end();
			++iter)
		{
			(*iter)->PropertiesEditor();
		}
	}
	else if (asset_is_selected)
	{
		ImGui::Text("File name: %s", selected_asset.name.c_str());
		ImGui::Text("Full path: %s", selected_asset.full_path.c_str());
		ImVec2 size = {100,100};
		if(selected_asset.type == ResourceTexture::type && resource_selected)
			ImGui::Image((ImTextureID)resource_selected->mini_image_buffer, size);
	}
	ImGui::End();
}

const ComponentTransform * PanelProperties::GetSelecteTransform()
{
	return selected_transform;
}

void PanelProperties::SetFileSelected(AssetFile selected_asset)
{
	if (selected_asset.type == ResourceTexture::type && selected_asset.uid == INVALID_RESOURCE_UID)
	{
		JSONFile meta_file;
		meta_file.LoadFile(selected_asset.full_path + "." + META_EXTENSION);
		UID uid = App->resource_manager->LoadUID(&meta_file);
		resource_selected = (ResourceTexture*)App->resource_manager->GetResource(uid);
	}
	this->selected_asset = selected_asset;
	asset_is_selected = true;
}


