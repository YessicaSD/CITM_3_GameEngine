#ifndef __IMGUI_TIMELINE_H__
#define __IMGUI_TIMELINE_H__

#include "imconfig.h"
#include <string>

//Timeline implementation for imgui
//Taken from
//https://github.com/nem0/LumixEngine/blob/timeline_gui/external/imgui/imgui_user.inl#L814
//https://github.com/ocornut/imgui/issues/772

namespace ImGui
{
	bool BeginTimeline(const char* str_id, float max_value);
	bool TimelineChannel(const char * channel_name, std::string * tooltips, float* values, int num_values);
	void TimelineMarker(float curr_time);
	void EndTimeline();
}

#endif
