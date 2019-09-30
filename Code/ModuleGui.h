
#define CURRENT_FPS_MAX_VALUE 101

#ifndef MODULEGUI_H_
#define MODULEGUI_H_
#include "Module.h"
#include "imgui\imgui.h"
#include "Timer.h"



class ModuleGUI : public Module
{
private:
	float fpsHistory[CURRENT_FPS_MAX_VALUE];
	float msHistory[CURRENT_FPS_MAX_VALUE];
	float RamHistory[CURRENT_FPS_MAX_VALUE];

	Timer updateGraph;
	

public:
	ModuleGUI(bool start_enabled = true);
	bool Init() override;
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	bool CleanUp() override;

private:
	void DisplayMainMenuBar(update_status &ret, bool &display_confi);
	void DisplayConfiguration(update_status &ret, bool &display_confi);
};
#endif // !MODULEGUI_H_

