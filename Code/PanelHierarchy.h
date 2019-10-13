#ifndef __PANEL_HIREARCHY_H_
#define __PANEL_HIREARCHY_H_

#include "Panel.h"

class GameObject;
class ComponentTransform;

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(std::string name, bool active, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
	void TreeEntry(ComponentTransform * gameobject);
};

#endif