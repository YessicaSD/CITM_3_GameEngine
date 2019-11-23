#include "ModuleTime.h"
#include "Application.h"
#include "Event.h"
ModuleTime::ModuleTime(const char * name):Module(name)
{
}

update_status ModuleTime::PreUpdate()
{
	real_time_delta_time = App->GetDt();
	++frame_count;
	switch (App->GetState())
	{
	case Application::State::PLAY:
		delta_time = real_time_delta_time;
		time += delta_time;
		break;
	case Application::State::PAUSE:
		delta_time = 0.f;
		break;
	default:
		break;
	}

	return update_status::UPDATE_CONTINUE;
}

void ModuleTime::EventRequest(const Event & event)
{
	if (event.type == Event::EVENT_TYPE::STOP)
	{
		time = 0.f;
	}
}

float ModuleTime::GetTime()
{
	return time;
}
