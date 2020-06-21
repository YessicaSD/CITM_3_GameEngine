#ifndef __PANEL_HIERARCHY_H_
#define __PANEL_HIERARCHY_H_

#include "Panel.h"
#include "../imgui/imgui.h"

class GameObject;
class ComponentTransform;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(std::string name, bool active, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
	void ChangeHierarchy();
	
private:
	ImVec4 hover_color;
	void DisplayChildren(ComponentTransform * gameobject);
	void SetDragAndDrop(ComponentTransform* object);
	void DragObject(ComponentTransform* object);
	void DropObject(ComponentTransform* object);

	ComponentTransform* drag_object = nullptr;
	ComponentTransform* target_object = nullptr;

	friend class ModuleGui;
};

#endif