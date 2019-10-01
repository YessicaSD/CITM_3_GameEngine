#ifndef PANEL_CONSOLE_H_
#define	PANEL_CONSOLE_H_
#include "Panel.h"
#include "imgui\imgui.h"
class PanelConsole : public Panel
{
private:
	ImGuiTextBuffer output;
public:
	PanelConsole(std::string name, bool active = false);
	~PanelConsole();

	void Log(const char* sentence);
	void Draw() override;
};
#endif // !PANEL_CONSOLE_H_

