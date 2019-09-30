#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include <vector>
#include "Module.h"
#include "Tool.h"

class ModuleEditor : public Module
{
public:
	update_status PreUpdate() override;

private:
	void CheckForShortcuts();

private:
	std::vector<Tool> tools;
};

#endif