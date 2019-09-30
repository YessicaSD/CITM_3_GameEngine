#include "ModuleEditor.h"

update_status ModuleEditor::PreUpdate()
{
	CheckForShortcuts();
	return UPDATE_CONTINUE;
}

void ModuleEditor::CheckForShortcuts()
{
	for (std::vector<Tool>::iterator iter = tools.begin(); iter != tools.end(); ++iter)
	{
		(*iter).CheckShortcut();
	}
}
