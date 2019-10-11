#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "ComponentTransform.h"
#include <vector>
#include <string>

class GameObject
{
public:
	GameObject(std::vector components = {Transform});

public:
	Transform transform;
	std::string name;
	bool active = true;
	std::vector<Component*> components;
	virtual void Disable() {};
	virtual void Enable() {};
};

#endif // !GAMEOBJECT_H_

