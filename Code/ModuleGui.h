
#ifndef MODULEGUI_H_
#define MODULEGUI_H_




#include "Module.h"

#include "imgui\imgui.h"
#include <vector>
#include "TabPanel.h"

class Timer;
class Panel;
class Shortcut;
class PanelConfiguration;
class PanelConsole;
class PanelShortcuts;
class PanelProperties;
class PanelAbout;
struct TabPanel;

enum class TYPE_TAB_PANEL
{
	RIGHT_TAB_PANEL = 0u,
	MAX_TAB_PANEL
};
class ModuleGui : public Module
{
private:
	bool showMenuImGui = false;
	std::vector<Panel*> panels;
	PanelConfiguration* panel_config = nullptr;
	PanelConsole* panel_console = nullptr;
	PanelShortcuts * panel_shortcuts = nullptr;
	PanelProperties* panel_properties = nullptr;
	PanelAbout* panel_about = nullptr;
	std::vector<Shortcut *> shortcuts;



public:
	ModuleGui(bool start_enabled = true);
	bool Init() override;
	update_status PreUpdate() override;
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	bool CleanUp() override;
	bool Log(const char*);

	void AddInputLog(SDL_Scancode key, KEY_STATE state);
	void ModifyShortcut(SDL_Scancode key);
	void SetTabPanelsResized(int width, int height);
private:
	void DisplayMainMenuBar(update_status &ret);



	TabPanel TabPanels[(uint)TYPE_TAB_PANEL::MAX_TAB_PANEL];
	friend class Shortcut;
	friend class Panel;
	friend class PanelShortcuts;

};
#endif // !MODULEGUI_H_

