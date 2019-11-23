#ifndef MODULE_TIME_H_
#define MODULE_TIME_H_
#include "Module.h"

class ModuleTime : public Module
{
private:
	int frame_count = 0; // app graphics frames since game start
	float time = 0.0f; // second since game start (Game Clock)
	float time_scale = 0.0f; //scale at which time is passing (Game Clock)
	float delta_time = 0.0f; // last frame time expressed in seconds (Game Clock)
	float real_time_since_startup = 0.0f; // seconds since game start (Real Time Clock)
	float real_time_delta_time = 0.0f; // last frame time expressed in seconds (Real Time Clock)
};


#endif // !MODULE_TIME_H_

