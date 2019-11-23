#ifndef  EVENT_H_
#define  EVENT_H_
struct Event
{
public:
	enum EVENT_TYPE
	{
		DROPPED_FILE,
		UPDATE_OCTREE,
		CHANGE_HIERARCHY,
		PLAY,
		STOP,
		PAUSE,
		UNPAUSE,
	}type;
	const char* path;
	Event(EVENT_TYPE type) : type(type)
	{}
	Event(EVENT_TYPE type, const char* path) : type(type), path(path)
	{}
};

#endif // ! EVENT_H_

