#include "PanelAssets.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ResourceTexture.h"
#include "ModuleGui.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleResourceManager.h"
//#include "mmgr/mmgr.h"

#include "PhysFS/include/physfs.h"

PanelAssets::PanelAssets(std::string name, bool state, std::vector<SDL_Scancode> shortcuts) :Panel(name, state, shortcuts)
{
}

void PanelAssets::Draw()
{
	ImGui::Begin(name.c_str());

	//if (ImGui::Button("Refresh"))
	//{
	//	//Checks for new assets
	//	//Compares the previous asset list with the new list
	//	//Adds the new files to the asset list
	//}

	//TODO: Deletes the whole Resources folder and creates all new files
	//if(ImGui::Button("Reimport"))

	DisplayFolderAssetsRecursive(App->resource_manager->asset_dir);

	ImGui::End();
}

void PanelAssets::DisplayFolderAssetsRecursive(AssetDir * dir)
{
	ImGuiTreeNodeFlags asset_tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
	for (auto iter = dir->assets.begin(); iter != dir->assets.end(); ++iter)
	{
		bool open = ImGui::TreeNodeEx((*iter)->name.c_str(), asset_tree_flags);
		//TODO: Check if this asset is a model
		DragAsset((*iter));
		if (open)
		{
			selected_asset = (*iter);
			ImGui::TreePop();
		}
	}
	
	ImGuiTreeNodeFlags dir_tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (dir->dirs.size() == 0)
	{
		dir_tree_flags |= ImGuiTreeNodeFlags_Leaf;
	}

	for (auto iter = dir->dirs.begin(); iter != dir->dirs.end(); ++iter)
	{
		if (ImGui::TreeNodeEx((*iter)->name.c_str(), dir_tree_flags))
		{
			DisplayFolderAssetsRecursive((*iter));
			ImGui::TreePop();
		}
	}
}

void PanelAssets::DragAsset(AssetFile * asset)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("asset", &asset, sizeof(AssetFile*));
		ImGui::EndDragDropSource();
	}
}

//TODO: Drop objects onto other directories on the assets and move their .meta files with them
//void PanelAssets::DropObject(AssetFile * asset)
//{
//	if (ImGui::BeginDragDropTarget())
//	{
//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("asset"))
//		{
//			AssetFile* payload_n = *(AssetFile**)payload->Data;
//			App->AddEvent(Event(Event::DROPPED_MODEL_TO_SCENE));
//		}
//		ImGui::EndDragDropTarget();
//	}
//}

AssetFile PanelAssets::GetAssetFile()
{
	return *selected_asset;
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