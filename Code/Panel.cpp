#include "Panel.h"
#include "Application.h"

Panel::Panel(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	name(name),
	active(active)
{
	shortcut.name = name;
	shortcut.keys = shortcuts;

	//App->gui->panels.push_back(this);
}

void Panel::SetActive(bool value)
{
	active = value;
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
	return (shortcut.keys.size() > 0u && shortcut.keys[0] != SDL_SCANCODE_UNKNOWN);
}
void Panel::SetName(std::string name)
{
	this->name = name;
}