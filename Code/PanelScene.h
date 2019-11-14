#ifndef __PANEL_SCENE_H__
#define __PANEL_SCENE_H__

#include "Panel.h"
#include "imgui/imgui.h"
#include "MathGeoLib/include/Math/float2.h"
class PanelScene : public Panel
{
public:
	PanelScene(std::string name, bool active, std::vector<SDL_Scancode> shortcuts);
	void Draw() override;

public:
	ImVec2 current_viewport_size;
	float2 cursor;
	int width, height;
	bool mouse_is_hovering;
private:
	void GetSizeWithAspectRatio(int current_width, int current_height, int wanted_width, int wanted_height, int& new_width, int& new_height);

};

#endif