
#ifndef _PANEL_H_
#define _PANEL_H_

#include <string>
#include <vector>
#include "..\SDL\include\SDL_scancode.h"
#include "Shortcut.h"

class Panel
{
public:
	Shortcut* shortcut = nullptr;
	float width, height;
protected:
	bool active = false;
	std::string name = "";

public:
	Panel(std::string name, bool active, std::vector<SDL_Scancode> shortcuts = {});

	void SetActive(bool value);
	void SwitchActive();
	bool IsActive() const;
	bool HasShortcut();
	void SetName(std::string name);
	const char* GetName()
	{
		return name.c_str();
	}
	virtual void Draw() {
	
	}
};
#endif // !_PANEL_H_
