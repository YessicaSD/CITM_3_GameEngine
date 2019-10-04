#ifndef __PANEL_SHORTCUTS_H__
#define __PANEL_SHORTCUTS_H__

#include "Panel.h"
#include "Globals.h"

#define KEYS_BUFFER_SIZE 128u

class PanelShortcuts : public Panel
{
public:
	PanelShortcuts(std::string name, bool active = false, std::vector<SDL_Scancode> shortcuts = {});

	void Draw() override;
	void ModifyShortcut(SDL_Scancode key);

private:
	void ShowModifyShortcutPanel();
	const char * GetKeysCharPtr(std::vector<SDL_Scancode> keys, char * buffer, const uint buffer_size);

private:
	//Modify shortcut
	bool modifying_shortcut = false;
	Shortcut * shortcut_to_modify = nullptr;
	std::vector<SDL_Scancode> new_key_combination;
};

#endif