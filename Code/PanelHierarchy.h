#ifndef __PANEL_HIERARCHY_H_
#define __PANEL_HIERARCHY_H_

#include "Panel.h"

class GameObject;
class ComponentTransform;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(std::string name, bool active, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
	void DisplayChildren(ComponentTransform * gameobject);
	
};

#endif