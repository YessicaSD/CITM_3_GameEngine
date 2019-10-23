#ifndef __PANEL_HIERARCHY_H_
#define __PANEL_HIERARCHY_H_

#include "Panel.h"
#include "imgui/imgui.h"

class GameObject;
class ComponentTransform;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(std::string name, bool active, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
	void DisplayChildren(ComponentTransform * gameobject);
	
private:
	const ImVec4 hover_color;

	friend class ModuleGui;
};

#endif