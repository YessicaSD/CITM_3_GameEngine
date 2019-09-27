#ifndef MODULEGUI_H_
#define MODULEGUI_H_
#include "Module.h"
#include "imgui\imgui.h"
#include "Timer.h"
#define CURRENT_FPS_MAX_VALUE 101

class ModuleGUI : public Module
{
private:
	float fpsHistory[CURRENT_FPS_MAX_VALUE];
	float msHistory[CURRENT_FPS_MAX_VALUE];

	Timer updateGraph;
	

public:
	ModuleGUI(Application* app, bool start_enabled = true);
	bool Init() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

private:
	void DisplayMainMenuBar(update_status &ret, bool &display_confi);
	void DisplayConfiguration(update_status &ret, bool &display_confi);
};
#endif // !MODULEGUI_H_

