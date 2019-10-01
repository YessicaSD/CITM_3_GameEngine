#ifndef PANEL_CONFIGURATION_H_
#define PANEL_CONFIGURATION_H_

#include "Panel.h"
#include "Timer.h"
#define CURRENT_FPS_MAX_VALUE 101

class PanelConfiguration : public Panel
{
	
private:
	Timer updateGraph;
	float fpsHistory[CURRENT_FPS_MAX_VALUE];
	float msHistory[CURRENT_FPS_MAX_VALUE];
	float RamHistory[CURRENT_FPS_MAX_VALUE];
public:
	PanelConfiguration(std::string name, bool active = false, std::vector<SDL_Scancode> shortcut = { SDL_SCANCODE_UNKNOWN });
	void Draw() override;
};
#endif // !PANEL_CONFIGURATION_H_

