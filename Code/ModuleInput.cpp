#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "manymouse-2e0dbebdbd28/manymouse.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	numMice = ManyMouse_Init();
	LOG("Number of mice: %i", numMice);

	if (numMice < 0)
	{
		LOG("ERROR LOADING MANYMOUSE LIBRERY");
		return false;
	}
	else if (numMice == 0)
	{
		LOG("NO MICE WERE DETECTED");
	}
	else
	{
		if (numMice > MAX_MICE)
		{
			numMice = MAX_MICE;
		}
	}

	for (int i = 0; i < numMice; ++i) {
		for (int j = 0; j < MAX_MOUSE_BUTTONS; ++j) {
			mice[i].mouse_buttons[j] = KEY_IDLE;
		}
	}

	return true;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	//Considers that only 1 event can be passed per frame
	for (int iterMouse = 0; iterMouse < numMice; ++iterMouse)
	{
		for (int j = 0; j < MAX_MOUSE_BUTTONS; ++j) {
			switch (mice[iterMouse].mouse_buttons[j]) {
			case KEY_DOWN:
				mice[iterMouse].mouse_buttons[j] = KEY_REPEAT;
				break;
			case KEY_UP:
				mice[iterMouse].mouse_buttons[j] = KEY_IDLE;
				break;
			}
		}
		mice[iterMouse].scrollState = SCROLL_IDLE;
	}

	//Reset mouse motion before polling for the new mouse motion
	for (int i = 0; i < numMice; ++i) {
		mice[i].x_motion = mice[i].y_motion = 0;
	}
	
	ManyMouseEvent event;
	while (ManyMouse_PollEvent(&event))
	{
		if (event.device >= (uint)numMice)
			continue;

		switch (event.type)
		{
		case MANYMOUSE_EVENT_RELMOTION:
		{
			if (event.item == 0) {
				mice[event.device].x_motion = event.value;
				mice[event.device].x += event.value;
			}
			else if (event.item == 1) {
				mice[event.device].y_motion = event.value;
				mice[event.device].y += event.value;
			}
		}
		break;
		case MANYMOUSE_EVENT_ABSMOTION:
		{
			//float val = (float)(event.value - event.minval);
			//float maxval = (float)(event.maxval - event.minval);
			//if (event.item == 0) {
			//	mice[event.device].x = (val / maxval) * SCREEN_WIDTH;
			//}
			//else if (event.item == 1) {
			//	mice[event.device].y = (val / maxval) * SCREEN_HEIGHT;
			//}
		}
		break;
		case MANYMOUSE_EVENT_SCROLL:
		{
			mice[event.device].scrollState = (SCROLL_STATE)event.value;
		}
			break;
		case MANYMOUSE_EVENT_BUTTON:
		{
			//- event.value returns 1 when pressed (keydown), and 0 when released (keyup)
			if (event.value == 1) {
				mice[event.device].mouse_buttons[event.item] = KEY_DOWN;
			}
			else if (event.value == 0) {
				mice[event.device].mouse_buttons[event.item] = KEY_UP;
			}
		}
			break;
		case MANYMOUSE_EVENT_DISCONNECT:

			break;
		}
		
	}


	const Uint8* keys = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	//Quit and resize (SDL events)
	bool quit = false;
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) {
			quit = true;
		}
		else if (e.type == SDL_WINDOWEVENT) {
			if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
				App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}
	if (quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP) {
		return UPDATE_STOP;
	}


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	ManyMouse_Quit();
	return true;
}