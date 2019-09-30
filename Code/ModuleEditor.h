#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include <vector>
#include "Module.h"

class Tool;

//Class which handles
//- Activating tools when their shortcut is pressed

class ModuleEditor : public Module
{
public:

	bool Start() override;
	update_status PreUpdate() override;
	update_status Update(float dt) override;
	void AddTool(Tool * tool);

private:
	void CheckForShortcuts();

	//Tool functions
	bool ActivateUndefinedTool();
	bool UpdateUndefinedTool();
	bool DeactivateUndefinedTool();

private:
	std::vector<Tool *> tools;

	friend class Tool;
};

#endif