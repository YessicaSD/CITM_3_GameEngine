#include "Tool.h"
#include "Application.h"
#include "ModuleInput.h"

Tool::Tool(std::string name, std::vector<SDL_Scancode> shortcut)
{

}

std::string Tool::GetName()
{
	return tool_name;
}

std::vector<SDL_Scancode> Tool::GetShortcut()
{
	return shortcut;
}

bool Tool::CheckShortcut()
{
	bool ret = true;

	//if (shortcut.size > 0)
	//{
	//	std::vector<SDL_Scancode>::iterator iter;
	//	for (iter = shortcut.begin();
	//		iter != shortcut.end() - 1 && ret;
	//		++iter)
	//	{
	//		KEY_STATE state = App->input->GetKey(*iter);

	//		if (state == KEY_DOWN || state == KEY_REPEAT)
	//		{
	//			ret = false;
	//		}
	//	}

	//	//Check if last shortcut letter has been pressed down (we don't want to repeat the shortcut action every frame the key combination is held down)
	//	if (ret && App->input->GetKey(*iter) == KEY_DOWN)
	//	{
	//		ret = true;
	//	}
	//}
	//else
	//{
	//	ret = false;
	//}

	return ret;
}
