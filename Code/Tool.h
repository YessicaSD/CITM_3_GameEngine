#ifndef __TOOL_H__
#define __TOOL_H__

#include <vector>
#include "SDL/include/SDL_scancode.h"
#include <string>
#include "ModuleEditor.h"

class Tool
{
public:
	Tool (std::string name, std::vector<SDL_Scancode> shortcut,
		bool (ModuleEditor::*ActivateTool)() = &ModuleEditor::ActivateUndefinedTool,
		bool (ModuleEditor::*DeactivateTool)() = &ModuleEditor::DeactivateUndefinedTool,
		bool (ModuleEditor::*UpdateTool)() = &ModuleEditor::UpdateUndefinedTool);
	std::string GetName();
	std::vector<SDL_Scancode> GetShortcut();

	bool CheckShortcut();
	bool IsActive();

public:
	std::vector<SDL_Scancode> shortcut;

	bool (ModuleEditor::*ActivateTool)();//Executed when the tool is activated
	bool (ModuleEditor::*UpdateTool)();//Executed every frame the tool is active
	//TODO: Maybe this should recieve dt
	bool (ModuleEditor::*DeactivateTool)();//Executed when the tool is deactivated

	//Used for searching the tool via de find menu
	std::string tool_name;

private:
	bool isActive = false;
};

#endif