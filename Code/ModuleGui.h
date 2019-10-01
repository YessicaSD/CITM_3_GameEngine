
#ifndef MODULEGUI_H_
#define MODULEGUI_H_




#include "Module.h"
#include "imgui\imgui.h"

#include <vector>

class Timer;
class Panel;
class Shortcut;
class PanelConfiguration;
class PanelConsole;

class ModuleGui : public Module
{
private:
	bool showMenuImGui = false;
	std::vector<Panel*> panels;
	PanelConfiguration* conf = nullptr;
	PanelConsole* console = nullptr;

	std::vector<Shortcut *> shortcuts;

public:
	ModuleGui(bool start_enabled = true);
	bool Init() override;
	update_status PreUpdate() override;
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	bool CleanUp() override;
	void Log(const char*);

public:
	void AddInputLog(SDL_Scancode key, KEY_STATE state);

private:
	void DisplayMainMenuBar(update_status &ret);

	friend class Shortcut;
	friend class Panel;
	friend class PanelShortcuts;

};
#endif // !MODULEGUI_H_

