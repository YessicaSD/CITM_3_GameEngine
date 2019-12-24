#ifndef __IMGUI_TIMELINE_H__
#define __IMGUI_TIMELINE_H__

#include "imconfig.h"

//Timeline implementation for imgui
//Taken from
//https://github.com/nem0/LumixEngine/blob/timeline_gui/external/imgui/imgui_user.inl#L814
//https://github.com/ocornut/imgui/issues/772

namespace ImGui
{
	bool BeginTimeline(const char* str_id, float max_value);
	bool TimelineEvent(const char* str_id, float* values);
	void EndTimeline();
}

#endif
