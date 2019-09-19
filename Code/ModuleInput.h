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
	int connected = 0;
	int x = 0, y = 0;
	int x_motion = 0, y_motion = 0;//The motion of the mouse in each axis from the last frame
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS] = { KEY_IDLE ,KEY_IDLE ,KEY_IDLE ,KEY_IDLE };
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

	KEY_STATE GetKey(int id=0) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton( int id, int device=0) const
	{
		return mice[device].mouse_buttons[id];
	}

	SCROLL_STATE GetScrollWheelState(int device=0) const
	{
		return mice[device].scrollState;
	}

	int GetMouseXMotion(uint device=0) {
		return mice[device].x_motion;
	}
	
	int GetMouseYMotion(uint device=0) {
		return mice[device].y_motion;
	}

private:
	KEY_STATE* keyboard;
	int numMice = 0;
	Mouse mice[MAX_MICE];
};