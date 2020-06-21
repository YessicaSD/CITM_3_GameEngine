#ifndef __PANEL_GAME_H__
#define __PANEL_GAME_H__

#include "Panel.h"
#include "../imgui/imgui.h"
#include "../MathGeoLib/include/Math/float2.h"
#include "../imGuizmo/ImGuizmo.h"
class ComponentCamera;
class ComponentTransform;

class PanelGame : public Panel
{
public:
	PanelGame(std::string name, bool active, std::vector<SDL_Scancode> shortcuts);
	void Draw() override;

public:
	ImVec2 current_viewport_size = { 0,0 };
	float2 cursor = float2::zero;
	int width = 0, height = 0;
	bool mouse_is_hovering = false;

	friend class ModuleGui;
	friend class ModuleScene;
	friend class ModuleCamera3D;
};

#endif