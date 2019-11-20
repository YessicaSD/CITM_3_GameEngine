#include "PanelAssets.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ResourceTexture.h"
#include "ModuleGui.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"
//#include "mmgr/mmgr.h"

#include "PhysFS/include/physfs.h"

PanelAssets::PanelAssets(std::string name, bool state, std::vector<SDL_Scancode> shortcuts) :Panel(name, state, shortcuts)
{
}

void PanelAssets::Draw()
{
	ImGui::Begin(name.c_str());

	if (ImGui::Button("Refresh"))
	{
		//Checks for new assets
		//Compares the previous asset list with the new list
		//Adds the new files to the asset list
	}

	//TODO: This shouldn't be done every frame
	App->resource_manager->asset_dir = new Dir();
	App->resource_manager->asset_dir->name = ASSETS_FOLDER;
	FillAssetTreeRecursive(App->resource_manager->asset_dir);

	DisplayFolderAssetsRecursive(App->resource_manager->asset_dir);

	DeleteTreeRecursive(App->resource_manager->asset_dir);

	ImGui::End();
}

//TODO: Fill asset tree the first time
void PanelAssets::FillAssetTreeRecursive(Dir * dir)
{
	std::vector<std::string> file_list;
	std::vector<std::string> dir_list;

	App->file_system->GetFilesAndDirs(dir->name.c_str(), file_list, dir_list);

	//Add files
	for (auto iter = file_list.begin(); iter != file_list.end(); ++iter)
	{
		Asset * new_asset = new Asset();
		new_asset->name = (*iter);
		dir->assets.push_back(new_asset);
	}

	//Add dirs
	for (auto iter = dir_list.begin(); iter != dir_list.end(); ++iter)
	{
		Dir * new_dir = new Dir();
		new_dir->name = (*iter);
		FillAssetTreeRecursive(new_dir);
		dir->dirs.push_back(new_dir);
	}
}

//TODO: Compare

//TODO: Delete for every new
//Instead of adding, compare and import the new files

//Delte tree recursively

void PanelAssets::DeleteTreeRecursive(Dir * dir)
{
	for (auto iter = dir->assets.begin(); iter != dir->assets.end(); ++iter)
	{
		delete((*iter));
	}

	for (auto iter = dir->dirs.begin(); iter != dir->dirs.end(); ++iter)
	{
		DeleteTreeRecursive((*iter));
	}
	delete(dir);
}

void PanelAssets::DisplayFolderAssetsRecursive(Dir * dir)
{
	ImGuiTreeNodeFlags asset_tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
	for (auto iter = dir->assets.begin(); iter != dir->assets.end(); ++iter)
	{
		if (ImGui::TreeNodeEx((*iter)->name.c_str(), asset_tree_flags))
		{
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
		bool is_open = ImGui::TreeNodeEx((*iter)->name.c_str(), dir_tree_flags);
		DisplayFolderAssetsRecursive((*iter));
		if (is_open)
		{
			ImGui::TreePop();
		}
	}
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