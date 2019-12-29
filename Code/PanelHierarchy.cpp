#include "PanelHierarchy.h"
#include "imgui/imgui.h"
#include "ModuleScene.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleGui.h"
#include "PanelProperties.h"
#include "imgui/imgui_internal.h"

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
		char buffer[UID_DIGITS];
		sprintf_s(buffer, "%020llu", (*iter)->gameobject->uid);
		is_open = ImGui::TreeNodeEx((std::string((*iter)->gameobject->GetName()) + "##" + buffer).c_str(), node_flags);
		SetDragAndDrop((*iter));
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

void PanelHierarchy::SetDragAndDrop(ComponentTransform * object)
{
	DragObject(object);
	DropObject(object);
}

void PanelHierarchy::DragObject(ComponentTransform * object)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("object", &object, sizeof(ComponentTransform*));
		ImGui::EndDragDropSource();
	}
}

void PanelHierarchy::DropObject(ComponentTransform * object)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("object"))
		{
			ComponentTransform* payload_n = *(ComponentTransform**)payload->Data;
			
			if (!payload_n->IsInChilds(object))
			{
				drag_object = payload_n;
				target_object = object;
				App->AddEvent(Event(Event::CHANGE_HIERARCHY));	
			}
			else
			{
				LOG("ERROR: Invalid Target."); 
			}
		}
		
		ImGui::EndDragDropTarget();
	}

	if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->Rect(), ImGui::GetID("Hierarchy")))
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("object"))
		{
			ComponentTransform* payload_n = *(ComponentTransform**)payload->Data;

			if (!payload_n->IsInChilds(App->scene->root_gameobject->transform))
			{
				drag_object = payload_n;
				target_object = App->scene->root_gameobject->transform;
				App->AddEvent(Event(Event::CHANGE_HIERARCHY));
			}
			else
			{
				LOG("ERROR: Invalid Target.");
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void PanelHierarchy::ChangeHierarchy()
{
	drag_object->parent->DeleteFromChildren(drag_object);
	target_object->AddChild(drag_object);
	drag_object->SetGlobalMatrix(target_object->global_matrix *  drag_object->local_matrix);

}