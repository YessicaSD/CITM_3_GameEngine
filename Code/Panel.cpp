#include "Panel.h"
#include "Application.h"

Panel::Panel(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	name(name),
	active(active)
{
	shortcut = new Shortcut(name, shortcuts);
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
	if (shortcut)
	{
		return (shortcut->keys.size() > 0u && shortcut->keys[0] != SDL_SCANCODE_UNKNOWN);
	}
	else
	{
		return false;
	}
}
void Panel::SetName(std::string name)
{
	this->name = name;
}