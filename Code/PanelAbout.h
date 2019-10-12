#ifndef PANEL_ABOUT_H_
#define PANEL_ABOUT_H_

#include "Panel.h"

class PanelAbout : public Panel
{
public:
	PanelAbout(const char* name, bool active = false, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
};




#endif // !PANEL_ABOUT_H_
