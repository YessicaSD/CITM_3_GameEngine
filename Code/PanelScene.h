#ifndef __PANEL_SCENE_H__
#define __PANEL_SCENE_H__

#include "Panel.h"
#include "imgui/imgui.h"

class PanelScene : public Panel
{
public:
	PanelScene(std::string name, bool active, std::vector<SDL_Scancode> shortcuts);
	void Draw() override;

public:
	ImVec2 current_viewport_size;
};

#endif