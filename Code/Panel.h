
#ifndef _PANEL_H_
#define _PANEL_H_

#include <string>
#include <vector>
#include "SDL\include\SDL_scancode.h"
#include "Shortcut.h"

class Panel
{
public:
	Shortcut shortcut;

protected:
	bool active = false;
	std::string name = "";

public:
	Panel(std::string name, bool active);
	Panel(std::string name, bool active, std::vector<SDL_Scancode> shortcuts);

	void SwitchActive();
	bool IsActive() const;
	void SetName(std::string name);
	virtual void Draw() {}
};
#endif // !_PANEL_H_
