#ifndef PANEL_CONFIGURATION_H_
#define PANEL_CONFIGURATION_H_

#include "Panel.h"
#include "Timer.h"
#include "imgui/imgui.h"

#define CURRENT_FPS_MAX_VALUE 101

enum KEY_STATE;

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration(std::string name, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;

	void AddInputLog(SDL_Scancode key, KEY_STATE state);

private:
	void RenderOption(const char * checkbox_string, bool *value, int gl_option);

private:
	ImGuiTextBuffer input_log_buffer;
	Timer updateGraph;
	float fpsHistory[CURRENT_FPS_MAX_VALUE];
	float msHistory[CURRENT_FPS_MAX_VALUE];
	float RamHistory[CURRENT_FPS_MAX_VALUE];
	bool scroll_input_log = false;
	int current_window_mode = 0;
	//Render options
	bool depth_test = false;
	bool cull_face = false;
	bool lighting = false;
	bool color_material = false;
	bool texture_2d = false;

};
#endif // !PANEL_CONFIGURATION_H_

