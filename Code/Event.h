#ifndef  __EVENT_H__
#define  __EVENT_H__

class ResourceModel;

struct Event
{
public:
	enum EVENT_TYPE
	{
		DROPPED_FILE,
		UPDATE_OCTREE,
		CHANGE_HIERARCHY,
		DROPPED_MODEL_TO_SCENE,
	}type;

	union 
	{
		struct
		{
			ResourceModel * model = nullptr;
		} drop_model_data;
	};

	const char* path;
	Event(EVENT_TYPE type) : type(type)
	{}
	Event(EVENT_TYPE type, const char* path) : type(type), path(path)
	{}
};

#endif // ! EVENT_H_

