
#ifndef MODULEGUI_H_
#define MODULEGUI_H_




#include "Module.h"
#include "imgui\imgui.h"

#include <vector>
class Timer;
class Panel;
class PanelConfiguration;

class ModuleGUI : public Module
{
private:
	bool showMenuImGui = false;
	std::vector<Panel*> panels;
	PanelConfiguration* conf=nullptr;

	std::vector<Panel * > panels;
	std::vector<Shortcut *> shortcuts;

public:
	ModuleGUI(bool start_enabled = true);
	bool Init() override;
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	bool CleanUp() override;
	void AddShortcut(Shortcut * shortcut);

private:
	void DisplayMainMenuBar(update_status &ret);
};
#endif // !MODULEGUI_H_

