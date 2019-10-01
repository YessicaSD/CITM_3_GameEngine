#include "Panel.h"
#include "Application.h"
#include "ModuleGui.h"

Panel::Panel(std::string name, bool active) :
	name(name),
	active(active)
{
	App->gui->panels.push_back(this);
}

Panel::Panel(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	name(name),
	active(active)
{
	shortcut.name = name;
	shortcut.keys = shortcuts;

	App->gui->panels.push_back(this);
}

void Panel::SwitchActive()
{
	active = !active;
}
bool Panel::IsActive() const
{
	return active;
}
bool Panel::HasShortcut()
{
	return (shortcut.keys.size() > 0 && shortcut.keys[0] != SDL_SCANCODE_UNKNOWN);
}
void Panel::SetName(std::string name)
{
	this->name = name;
}