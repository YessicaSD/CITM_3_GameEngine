#ifndef __TOOL_H__
#define __TOOL_H__

#include <vector>
#include "SDL/include/SDL_scancode.h"
#include <string>

class Tool
{
public:
	Tool::Tool(std::string name, std::vector<SDL_Scancode> shortcut);
	std::string GetName();
	std::vector<SDL_Scancode> GetShortcut();

	bool CheckShortcut();

public:
	std::vector<SDL_Scancode> shortcut;

	//Used for searching the tool via de find menu
	std::string tool_name;
};

#endif