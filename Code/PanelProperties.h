#ifndef __PANEL_PROPERTIES_H__
#define __PANEL_PROPERTIES_H__

#include "Panel.h"
#include <string>
class ComponentTransform;

class PanelProperties : public Panel
{
	ComponentTransform* selected_gameobject = nullptr;
public:
	PanelProperties(std::string name, bool state, std::vector<SDL_Scancode> shortcuts = {});
	void SetGameObject(ComponentTransform* gameobject);
	void Draw() override;
};



#endif // !PANEL_PROPERTIES_H_

