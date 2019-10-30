#include "Shortcut.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleGui.h"

Shortcut::Shortcut()
{
	App->gui->shortcuts.push_back(this);
}

Shortcut::Shortcut(std::string name, std::vector<SDL_Scancode> keys) :
	name(name),
	keys(keys)
{
	App->gui->shortcuts.push_back(this);
}

bool Shortcut::Pressed()
{
	bool ret = false;
	if (this == nullptr)
	{
		return false;
	}
	if (keys.size() > 0)
	{
		ret = true;
		std::vector<SDL_Scancode>::iterator iter;
		for (iter = keys.begin();
			iter != keys.end() - 1 && ret;
			++iter)
		{
			KEY_STATE state = App->input->GetKey(*iter);

			if (state == KEY_UP || state == KEY_IDLE)
			{
				ret = false;
			}
		}

		//Check if last shortcut letter has been pressed down (we don't want to repeat the shortcut action every frame the key combination is held down)
		if (ret && App->input->GetKey(*iter) == KEY_DOWN)
		{
			ret = true;
		}
		else
		{
			ret = false;
		}
	}

	return ret;
}

bool Shortcut::Held()
{
	bool ret = false;

	if (keys.size() > 0)
	{
		ret = true;
		std::vector<SDL_Scancode>::iterator iter;
		for (iter = keys.begin();
			iter != keys.end() && ret;
			++iter)
		{
			KEY_STATE state = App->input->GetKey(*iter);

			if (state == KEY_UP || state == KEY_IDLE)
			{
				ret = false;
			}
		}
	}

	return ret;
}