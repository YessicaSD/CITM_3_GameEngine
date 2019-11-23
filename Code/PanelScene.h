#ifndef __PANEL_SCENE_H__
#define __PANEL_SCENE_H__

#include "Panel.h"
#include "imgui/imgui.h"
#include "MathGeoLib/include/Math/float2.h"
#include "imGuizmo/ImGuizmo.h"
class ComponentCamera;
class ComponentTransform;
struct AssetFile;

class PanelScene : public Panel
{
public:
	PanelScene(std::string name, bool active, std::vector<SDL_Scancode> shortcuts);
	void Draw() override;

public:
	ImVec2 current_viewport_size = {0,0};
	float2 cursor = float2::zero;
	int width = 0, height = 0;
	bool mouse_is_hovering = false;
	void DrawGizmo(ComponentCamera* camera, ComponentTransform* selected_object);

private:
	void GetSizeWithAspectRatio(int current_width, int current_height, int wanted_width, int wanted_height, int& new_width, int& new_height);
	void DropObject();
	ImGuizmo::OPERATION guizmo_op = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE      guizmo_mode = ImGuizmo::LOCAL;
	bool is_over_gizmo = false;
	bool update_octree_when_stop_moving = false;
	bool is_being_used = false;
	friend class ModuleScene;
	friend class ModuleCamera3D;
};

#endif