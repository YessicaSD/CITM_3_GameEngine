#include "ModuleEditor.h"
#include "Tool.h"
#include "Application.h"
#include "ModuleCamera3D.h"

bool ModuleEditor::Start()
{
	new Tool("Navigate forward", { SDL_SCANCODE_W }, &ModuleEditor::ActivateUndefinedTool, &ModuleEditor::DeactivateUndefinedTool, &ModuleEditor::UpdateMoveForwardTool);
	new Tool("Navigate backward", { SDL_SCANCODE_S });
	new Tool("Navigate left", { SDL_SCANCODE_A });
	new Tool("Navigate right", { SDL_SCANCODE_D });

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
			(*iter)->ActivateTool;
			(*iter)->isActive = true;
		}
	}
}

update_status ModuleEditor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	for (std::vector<Tool*>::iterator iter = tools.begin();
		iter != tools.end() && ret == UPDATE_CONTINUE;
		++iter)
	{
		if ((*iter)->IsActive())
		{
			//TODO: Send dt
			ret = (*iter)->UpdateTool(dt) ? UPDATE_CONTINUE : UPDATE_STOP;
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

bool ModuleEditor::UpdateUndefinedTool(float dt)
{
	LOG("Executing undefined tool.");
	return true;
}

bool ModuleEditor::DeactivateUndefinedTool()
{
	LOG("Deactivating undefined tool.");
	return true;
}

bool ModuleEditor::UpdateMoveForwardTool(float dt)
{
	//TODO: Put this in a define
	const float camera_speed = 3.f;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		App->camera->Position += vec3(0, 0, camera_speed * dt);
	}
	return true;
}
