#ifndef __PANEL_SHORTCUTS_H__
#define __PANEL_SHORTCUTS_H__

#include "Panel.h"

class PanelShortcuts : public Panel
{
public:
	PanelShortcuts(std::string name, bool active = false, std::vector<SDL_Scancode> shortcuts = { SDL_SCANCODE_UNKNOWN });

	void Draw() override;
};

#endif