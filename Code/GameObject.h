#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_


#include "Transform.h"
#include <vector>
#include <string>

class GameObject
{
public:
	Transform transform;
	std::string name;
	bool active = true;
	std::vector<Component*> components;
	void Disable() {};
	void Enable() {};
};



#endif // !GAMEOBJECT_H_

