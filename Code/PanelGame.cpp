#include "PanelGame.h"
#include "imgui/imgui.h"
#include "Application.h"

#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleGui.h"
#include "ModuleCamera3D.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"

#include <algorithm>
#include "Event.h"

PanelGame::PanelGame(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	Panel(name, active, shortcuts)
{ }

//PanelScene is called by ModuleGui
//ModuleScene executes draws (fills the framebuffer texture) before ModuleGui
//By doing it in this order we can get the image of this frame
void PanelGame::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin("Game");

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	width = ImGui::GetWindowWidth();
	height = ImGui::GetWindowHeight();

	ImVec2 min = ImGui::GetCursorScreenPos();
	
	current_viewport_size = ImGui::GetContentRegionAvail();
	ImVec2 max = ImVec2(min.x + current_viewport_size.x, min.y + current_viewport_size.y);
	mouse_is_hovering = ImGui::IsMouseHoveringRect(min, max);
	cursor = { ImGui::GetMousePos().x - min.x,ImGui::GetMousePos().y - min.y };

	ImGui::GetWindowDrawList()->AddImage(
		(void*)App->renderer3D->game_fbo.render_texture,
		min,
		ImVec2(min.x + width, min.y + height),
		ImVec2(0, 1),
		ImVec2(1, 0));

	ImGui::End();
	ImGui::PopStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding);
}



