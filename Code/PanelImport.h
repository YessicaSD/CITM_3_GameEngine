#ifndef __PANEL_IMPORT_H__
#define __PANEL_IMPORT_H__

#include "Panel.h"

//Panel that lets you change file import options
class PanelImport : public Panel
{
public:
	PanelImport(std::string name, bool active = false, std::vector<SDL_Scancode> shortcuts = {});
	void Draw()override;
	void DrawTexture();
};

#endif