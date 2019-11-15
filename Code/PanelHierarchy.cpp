#include "PanelHierarchy.h"
#include "imgui/imgui.h"
#include "ModuleScene.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleGui.h"
#include "PanelProperties.h"

PanelHierarchy::PanelHierarchy(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	Panel(name, active, shortcuts),
	hover_color(220, 220, 220, 255)
{

}

void PanelHierarchy::Draw()
{
	hover_color = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_ButtonActive);//TODO: Put in constructor
	ImGui::Begin("Hierarchy");
	DisplayChildren(App->scene->root_gameobject->transform);
	ImGui::End();
}

void PanelHierarchy::DisplayChildren(ComponentTransform * transform)
{
	for (std::vector<ComponentTransform*>::iterator iter = transform->children.begin();
		iter != transform->children.end();
		++iter)
	{
		bool is_selected = false;
		bool is_open = false;
		bool is_clicked = false;
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if ((*iter)->children.size() == 0)
		{
			node_flags |= ImGuiTreeNodeFlags_Leaf;
		}
		is_selected = (*iter) == App->gui->panel_properties->selected_transform;
		if (is_selected)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, hover_color);
		}
		if ((*iter)->open_in_hierarchy)
		{
			ImGui::SetNextItemOpen(true);
			(*iter)->open_in_hierarchy = false;
		}
		is_open = ImGui::TreeNodeEx((*iter)->gameobject->GetName(), node_flags);

		if (is_selected)
		{
			ImGui::PopStyleColor();
		}
		is_clicked = ImGui::IsItemClicked(0);
		if (is_clicked)
		{
			App->gui->SetSelectedGameObjec((*iter));
		}
		if (is_open)
		{
			DisplayChildren((*iter));
			ImGui::TreePop();
		}
	}
}