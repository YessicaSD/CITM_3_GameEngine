#ifndef PANEL_ASSETS_H_
#define PANEL_ASSETS_H_

#include "Panel.h"
#include <string>
class PanelAssets : public Panel
{

public:
	float image_size = 100;
	PanelAssets(std::string name, bool state, std::vector<SDL_Scancode> shortcuts = {});
	void Draw();

	friend class ModuleGui;
};




#endif // !PANEL_ASSETS_H_
