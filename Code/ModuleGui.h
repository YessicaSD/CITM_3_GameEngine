#ifndef MODULEGUI_H_
#define MODULEGUI_H_
#include "Module.h"
#include "imgui\imgui.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
	bool Start() override;

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	update_status Update(float dt) override;
	
};
#endif // !MODULEGUI_H_

