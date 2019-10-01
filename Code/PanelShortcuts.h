#ifndef __PANEL_SHORTCUTS_H__
#define __PANEL_SHORTCUTS_H__

#include "Panel.h"

class PanelShortcuts : public Panel
{
public:
	PanelShortcuts(std::string name, bool active = false, std::vector<SDL_Scancode> shortcuts = { SDL_SCANCODE_UNKNOWN });

	void Draw() override;
	std::string ScancodeToString(std::vector<SDL_Scancode> keys);

private:
	void ShowModifyShortcutPanel();

private:

	//Modify shortcut
	bool modifying_shortcut = false;
	Shortcut * shortcut_to_modify = nullptr;
	std::vector<SDL_Scancode> new_key_combination;
};

#endif