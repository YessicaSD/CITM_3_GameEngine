#include "PanelScene.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include <algorithm>


PanelScene::PanelScene(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	Panel(name, active, shortcuts)
{

}

//PanelScene is called by ModuleGui
//ModuleScene executes draws (fills the framebuffer texture) before ModuleGui
//By doing it in this order we can get the image of this frame
void PanelScene::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin("Scene");

	float panel_width = ImGui::GetWindowWidth();
	float panel_height = ImGui::GetWindowHeight();
	GetSizeWithAspectRatio(App->window->GetWindowWidth(), App->window->GetWindowHeight(), panel_width-50, panel_height-50, width, height);
	
	current_viewport_size = ImGui::GetContentRegionAvail();
	ImVec2 min = ImGui::GetCursorScreenPos();
	min = { min.x + 25, min.y + 25 };
	ImVec2 max = ImVec2(min.x + current_viewport_size.x, min.y + current_viewport_size.y);
	bool mouse_is_hovering = ImGui::IsMouseHoveringRect(min, max);

	
	cursor = { ImGui::GetMousePos().x - min.x,ImGui::GetMousePos().y - min.y };
	
	ImGui::GetWindowDrawList()->AddImage(
		(void*)App->renderer3D->scene_fbo.render_texture,
		min,
		ImVec2(min.x + width,  min.y + height),
		ImVec2(0, 1),
		ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding);
}

void PanelScene::GetSizeWithAspectRatio(int current_width, int current_height, int wanted_width, int wanted_height, int & new_width, int & new_height)
{
	float scale_height = (float)wanted_height / (float)current_height;
	float scale_width = (float)wanted_width / (float)current_width;

	float scale = (std::min)(scale_height, scale_width);

	new_width = current_width * scale;
	new_height = current_height * scale;
}
