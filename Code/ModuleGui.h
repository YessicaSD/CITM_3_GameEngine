#ifndef MODULEGUI_H_
#define MODULEGUI_H_
#include "Module.h"
#include "imgui\imgui.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
	bool Start() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

private:
	void DisplayGui(update_status &ret);
};
#endif // !MODULEGUI_H_

