#ifndef  __EVENT_H__
#define  __EVENT_H__

class ResourceModel;
class GameObject;
struct Event
{
public:
	enum EVENT_TYPE
	{
		DROPPED_FILE,
		UPDATE_OCTREE,
		CHANGE_HIERARCHY,
		DROPPED_MODEL_TO_SCENE,
		PLAY,
		STOP,
		PAUSE,
		UNPAUSE,
		SAVE_SCENE,
		LOAD_SCENE,
		DELETE_OBJECT,
	}type;

	union 
	{
		struct
		{
			ResourceModel * model = nullptr;
		} drop_model_data;
		struct
		{
			GameObject* ptr = nullptr;
		}object;
	};

	const char* path;
	Event(EVENT_TYPE type) : type(type)
	{}
	Event(EVENT_TYPE type, const char* path) : type(type), path(path)
	{}
};

#endif // ! EVENT_H_

