#pragma once
#include "Module.h"
#include "Globals.h"
#include "manymouse-2e0dbebdbd28\manymouse.h"
#define MAX_MOUSE_BUTTONS 5
#define MAX_MICE 4

enum MOUSE_BUTTONS {
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_BACK,//Button on the left part of some mice
	MOUSE_FORWARD//Button on the left part of some mice
};

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum SCROLL_STATE
{
	SCROLL_FORWARD = 1,
	SCROLL_IDLE = 0,
	SCROLL_BACKWARD = -1
};

typedef struct Mouse
{
	int connected;
	int x, y;
	int x_motion, y_motion;//The motion of the mouse in each axis from the last frame
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	SCROLL_STATE scrollState = SCROLL_IDLE;
};

class ModuleInput : public Module
{
public:
	ModuleInput(Application* app, bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int device, int id) const
	{
		return mice[device].mouse_buttons[id];
	}

	SCROLL_STATE GetScrollWheelState(int device) const
	{
		return mice[device].scrollState;
	}

	int GetMouseXMotion(uint device) {
		return mice[device].x_motion;
	}
	
	int GetMouseYMotion(uint device) {
		return mice[device].y_motion;
	}

private:
	KEY_STATE* keyboard;
	int numMice = 0;
	Mouse mice[MAX_MICE];
};