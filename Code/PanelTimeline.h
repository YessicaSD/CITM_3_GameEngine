#ifndef __PANEL_TIMELINE_H__
#define __PANEL_TIMELINE_H__

#include "Panel.h"

class PanelTimeline : public Panel
{
public:
	PanelTimeline(std::string name, bool state, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;

	friend class ModuleGui;
};

#endif
