#include "ModuleEditor.h"
#include "Tool.h"

bool ModuleEditor::Start()
{
	new Tool("zoom", {SDL_SCANCODE_0, SDL_SCANCODE_1 });
	new Tool("pan", {SDL_SCANCODE_1});
	new Tool("rotate", {SDL_SCANCODE_2});

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	CheckForShortcuts();
	return UPDATE_CONTINUE;
}

void ModuleEditor::AddTool(Tool * tool)
{
	tools.push_back(tool);
}

void ModuleEditor::CheckForShortcuts()
{
	for (std::vector<Tool*>::iterator iter = tools.begin(); iter != tools.end(); ++iter)
	{
		if ((*iter)->CheckShortcut())
		{
			//Some tools deactivate other tools when they are activated
			//(*iter)->ActivateTool();
			(*iter)->ActivateTool;
		}
	}
}

update_status ModuleEditor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	for (std::vector<Tool*>::iterator iter = tools.begin();
		iter != tools.end() || ret != UPDATE_CONTINUE;
		++iter)
	{
		if ((*iter)->IsActive())
		{
			//ret = (*iter)->UpdateTool() ? UPDATE_CONTINUE : UPDATE_STOP;
			ret = (*iter)->UpdateTool ? UPDATE_CONTINUE : UPDATE_STOP;
		}
	}

	return ret;
}

bool ModuleEditor::CleanUp()
{
	//Delete all the tools

	return true;
}

bool ModuleEditor::ActivateUndefinedTool()
{
	LOG("Activating undefined tool.");
	return true;
}

bool ModuleEditor::UpdateUndefinedTool()
{
	LOG("Executing undefined tool.");
	return true;
}

bool ModuleEditor::DeactivateUndefinedTool()
{
	LOG("Deactivating undefined tool.");
	return true;
}
