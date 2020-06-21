#ifndef __PANEL_RESOURCES_H__
#define __PANEL_RESOURCES_H__

#include "Panel.h"

class PanelResources : public Panel
{

public:
	PanelResources(std::string name, bool state, std::vector<SDL_Scancode> shortcuts = {});
	void Draw();

	friend class ModuleGui;
};

#endif