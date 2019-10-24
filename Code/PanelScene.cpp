#include "PanelScene.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

PanelScene::PanelScene(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	Panel(name, active, shortcuts)
{

}

void PanelScene::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin("Scene");
	current_viewport_size = ImGui::GetContentRegionAvail();
	ImVec2 min = ImGui::GetCursorScreenPos();
	ImVec2 max = ImVec2(min.x + current_viewport_size.x, min.y + current_viewport_size.y);
	bool mouse_is_hovering = ImGui::IsMouseHoveringRect(min, max);
	ImGui::Image((ImTextureID)App->renderer3D->render_texture, ImVec2(current_viewport_size.x, current_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	ImGui::PopStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding);
}
