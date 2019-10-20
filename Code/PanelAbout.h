#ifndef PANEL_ABOUT_H_
#define PANEL_ABOUT_H_

#include "Panel.h"
#include <string>
class PanelAbout : public Panel
{
public:
	PanelAbout(std::string name, bool active = false, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
};




#endif // !PANEL_ABOUT_H_
