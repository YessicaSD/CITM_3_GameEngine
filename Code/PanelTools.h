#ifndef PANEL_TOOLS_H_
#define PANEL_TOOLS_H_
#include "Panel.h"
class ResourceTexture;
class PanelTools : public Panel
{
public:
	PanelTools(std::string name, bool active, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
private:
	ResourceTexture* atlas = nullptr;
	float button_width =  0.28; 
	float button_height = 0.28;
};




#endif // !PANEL_TOOLS_H_
