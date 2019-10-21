#include "PanelHierarchy.h"
#include "imgui/imgui.h"
#include "ModuleScene.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleGui.h"
#include "PanelProperties.h"

PanelHierarchy::PanelHierarchy(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) : Panel(name, active, shortcuts)
{

}

void PanelHierarchy::Draw()
{
	ImGui::Begin("Hierarchy");
	DisplayChildren(&App->scene->root_gameobject.transform);
	//for (std::vector<ComponentTransform*>::iterator iter = App->scene->root_gameobject.transform.children.begin();
	//	iter != App->scene->root_gameobject.transform.children.end();
	//	++iter)
	//{
	//	TreeEntry((*iter));
	//}
	//App->gui->panel_properties->SetGameObject(selected_object);
	//selected_object = nullptr;
	ImGui::End();

	//if (("Advanced, with Selectable nodes"))
	//{
	//	static bool align_label_with_current_x_position = false;
	//	ImGui::Checkbox("Align label with current X position)", &align_label_with_current_x_position);
	//	ImGui::Text("Hello!");
	//	if (align_label_with_current_x_position)
	//		ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	//	static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	//	int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	//	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	//	for (int i = 0; i < 6; i++)
	//	{
	//		// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
	//		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	//		if (selection_mask & (1 << i))
	//			node_flags |= ImGuiTreeNodeFlags_Selected;
	//		if (i < 3)
	//		{
	//			// Items 0..2 are Tree Node
	//			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
	//			if (ImGui::IsItemClicked())
	//				node_clicked = i;
	//			if (node_open)
	//			{
	//				ImGui::Text("Blah blah\nBlah Blah");
	//				ImGui::TreePop();
	//			}
	//		}
	//		else
	//		{
	//			// Items 3..5 are Tree Leaves
	//			// The only reason we use TreeNode at all is to allow selection of the leaf.
	//			// Otherwise we can use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
	//			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
	//			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
	//			if (ImGui::IsItemClicked())
	//				node_clicked = i;
	//		}
	//	}
	//	if (node_clicked != -1)
	//	{
	//		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
	//		if (ImGui::GetIO().KeyCtrl)
	//			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
	//		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
	//			selection_mask = (1 << node_clicked);           // Click to single-select
	//	}
	//	ImGui::PopStyleVar();
	//	if (align_label_with_current_x_position)
	//		ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	//	ImGui::TreePop();
	//}
}

void PanelHierarchy::DisplayChildren(ComponentTransform * transform)
{
	for (std::vector<ComponentTransform*>::iterator iter = transform->children.begin();
		iter != transform->children.end();
		++iter)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if ((*iter)->children.size() == 0)
		{
			node_flags |= ImGuiTreeNodeFlags_Leaf;
		}
		bool is_selected = (*iter) == App->gui->panel_properties->selected_transform;
		if (is_selected)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor(220, 220, 220, 255));
		}
		bool node_open = ImGui::TreeNodeEx((*iter)->gameobject->GetName(), node_flags);
		if (is_selected)
		{
			ImGui::PopStyleColor();
		}
		bool clicked = ImGui::IsItemClicked(0);
		if (clicked)
		{
			App->gui->panel_properties->SetSelectedTransform((*iter));
		}
		if (node_open)
		{
			DisplayChildren((*iter));
			ImGui::TreePop();
		}
	}
}