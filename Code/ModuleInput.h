#pragma once
#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_wheel;
	int mouse_x_motion;
	int mouse_y_motion;
	char *dropped_filedir;
	//int mouse_z_motion;

public:
	
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init() override;
	update_status PreUpdate() override;
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseWheel() const
	{
		return mouse_wheel;
	}

	int GetMouseMotionX() const
	{
		return mouse_x_motion;
	}

	int GetMouseMotionY() const
	{
		return mouse_y_motion;
	}


};