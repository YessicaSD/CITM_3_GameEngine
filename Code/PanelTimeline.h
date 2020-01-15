#ifndef __PANEL_TIMELINE_H__
#define __PANEL_TIMELINE_H__

#include "Panel.h"
#include "ResourceAnimation.h"

class PanelTimeline : public Panel
{
public:
	PanelTimeline(std::string name, bool state, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
private:
	void DisplayTransformationChannel3Coord(const char * channel_name, int num_values, KeyAnimation<math::float3>* keys);
	void DisplayTransformationChannel4Coord(const char * channel_name, int num_values, KeyAnimation<math::Quat>* keys);

	friend class ModuleGui;
};

#endif
