#include "imgui_timeline.h"
#include "imgui.h"
#include "imgui_internal.h"

#define HEADER_TEXT_OFFSET 100

namespace ImGui
{
	static float s_max_timeline_value;

	bool BeginTimeline(const char* str_id, float max_value)
	{
		s_max_timeline_value = max_value;
		return BeginChild(str_id);
	}


	static const float TIMELINE_RADIUS = 6;


	bool TimelineChannel(const char * channel_name, std::string * tooltips, float* values, int num_values)
	{
		ImGuiWindow* win = GetCurrentWindow();
		const ImU32 inactive_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Button]);
		const ImU32 active_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_ButtonHovered]);
		bool changed = false;
		ImVec2 cursor_pos = win->DC.CursorPos;

		ImGui::Text(channel_name);

		for (int i = 0; i < num_values; ++i)
		{
			ImVec2 pos = cursor_pos;
			pos.x += HEADER_TEXT_OFFSET + (win->Size.x - HEADER_TEXT_OFFSET) * (values[i] / s_max_timeline_value) + TIMELINE_RADIUS;
			pos.y += TIMELINE_RADIUS;

			SetCursorScreenPos(pos - ImVec2(TIMELINE_RADIUS, TIMELINE_RADIUS));
			PushID(i);
			InvisibleButton(tooltips->c_str(), ImVec2(2 * TIMELINE_RADIUS, 2 * TIMELINE_RADIUS));//TODO: Add unique id in str_id
			if (IsItemActive() || IsItemHovered())
			{
				ImGui::SetTooltip(tooltips[i].c_str());
			}
			if (IsItemActive() && IsMouseDragging(0))
			{
				values[i] += GetIO().MouseDelta.x / win->Size.x * s_max_timeline_value;
				changed = true;
			}
			PopID();
			win->DrawList->AddCircleFilled(
				pos,
				TIMELINE_RADIUS,
				IsItemActive() || IsItemHovered() ? active_color : inactive_color);
		}

		return changed;
	}


	void TimelineMarker(float curr_time)
	{
		ImGuiWindow* win = GetCurrentWindow();

		ImU32 line_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Button]);
		ImVec2 start(
			HEADER_TEXT_OFFSET + GetWindowContentRegionMin().x + win->Pos.x,
			GetWindowContentRegionMax().y - GetTextLineHeightWithSpacing() + win->Pos.y);

		ImVec2 a = ImVec2(HEADER_TEXT_OFFSET, 0) + GetWindowContentRegionMin() + win->Pos + ImVec2(TIMELINE_RADIUS, 0);
		a.x += curr_time / s_max_timeline_value * (GetWindowContentRegionWidth() - HEADER_TEXT_OFFSET);
		ImVec2 b = a;
		b.y = start.y;
		win->DrawList->AddLine(a, b, line_color);
	}

	void EndTimeline()
	{
		ImGuiWindow* win = GetCurrentWindow();

		ImU32 color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Button]);
		ImU32 line_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Border]);
		ImU32 text_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Text]);
		float rounding = GImGui->Style.ScrollbarRounding;
		ImVec2 start(
			HEADER_TEXT_OFFSET + GetWindowContentRegionMin().x + win->Pos.x,
			GetWindowContentRegionMax().y - GetTextLineHeightWithSpacing() + win->Pos.y);
		ImVec2 end = GetWindowContentRegionMax() + win->Pos;

		win->DrawList->AddRectFilled(start, end, color, rounding);

		const int LINE_COUNT = 5;
		const ImVec2 text_offset(0, GetTextLineHeightWithSpacing());
		for (int i = 0; i < LINE_COUNT; ++i)
		{
			ImVec2 a = ImVec2(HEADER_TEXT_OFFSET, 0) + GetWindowContentRegionMin() + win->Pos + ImVec2(TIMELINE_RADIUS, 0);
			a.x += i * GetWindowContentRegionWidth() / LINE_COUNT;
			ImVec2 b = a;
			b.y = start.y;
			win->DrawList->AddLine(a, b, line_color);
			char tmp[256];
			ImFormatString(tmp, sizeof(tmp), "%.2f", i * s_max_timeline_value / LINE_COUNT);
			win->DrawList->AddText(b, text_color, tmp);
		}

		EndChild();
	}
}