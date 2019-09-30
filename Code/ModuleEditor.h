#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include <vector>
#include "Module.h"

#define CAMERA_SPEED 3.f;

class Tool;

//Class which handles
//- Activating tools when their shortcut is pressed

class ModuleEditor : public Module
{
public:

	bool Start() override;
	update_status PreUpdate() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	void AddTool(Tool * tool);

private:
	void CheckForShortcuts();

	//Tool functions
	bool ActivateUndefinedTool();
	bool UpdateUndefinedTool(float dt);
	bool DeactivateUndefinedTool();

	bool UpdateMoveForwardTool(float dt);

private:
	std::vector<Tool *> tools;

	friend class Tool;
};

#endif