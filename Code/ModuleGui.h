#ifndef MODULEGUI_H_
#define MODULEGUI_H_
#include "Module.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(Application* app, bool start_enabled = true);
	
};
#endif // !MODULEGUI_H_

