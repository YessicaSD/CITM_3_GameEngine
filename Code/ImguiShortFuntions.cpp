#include "imgui/imgui.h"


void MoveCursor(float x, float y)
{
	ImVec2 moved_pos = ImGui::GetCursorPos();
	moved_pos.x += x;
	moved_pos.y += y;
	ImGui::SetCursorPos(moved_pos);
}