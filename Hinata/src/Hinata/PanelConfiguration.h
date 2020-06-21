#ifndef PANEL_CONFIGURATION_H_
#define PANEL_CONFIGURATION_H_

#include "Panel.h"
#include "Timer.h"
#include "../imgui/imgui.h"

enum KEY_STATE;

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration(std::string name, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;

	static void RenderOption(const char * checkbox_string, bool *value, int gl_option);

private:
	
	friend class ModuleGui;
};
#endif // !PANEL_CONFIGURATION_H_

