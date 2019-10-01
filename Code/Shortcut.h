#ifndef __SHORTCUT_H__
#define __SHORTCUT_H__

#include <vector>
#include <string>
#include "SDL/include/SDL_scancode.h"

class Shortcut
{
public:
	Shortcut();
	Shortcut(std::string name, std::vector<SDL_Scancode> keys);
	bool Pressed();

public:
	std::string name;
	std::vector<SDL_Scancode> keys;

	friend class ModuleGui;
};

#endif