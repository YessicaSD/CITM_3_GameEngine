#ifndef PANEL_CONSOLE_H_
#define	PANEL_CONSOLE_H_
#include "Panel.h"
#include "../imgui\imgui.h"
class PanelConsole : public Panel
{
private:
	ImGuiTextBuffer output;
public:
	PanelConsole(std::string name, bool active = false, std::vector<SDL_Scancode> shortcuts = {});
	~PanelConsole();

	void Log(const char* sentence);
	void Draw() override;

	friend class ModuleGui;
};
#endif // !PANEL_CONSOLE_H_

