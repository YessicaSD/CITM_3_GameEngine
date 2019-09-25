#ifndef MODULEGUI_H_
#define MODULEGUI_H_
#include "Module.h"
#include "imgui\imgui.h"

class ModuleGUI : public Module
{
private:
	
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

