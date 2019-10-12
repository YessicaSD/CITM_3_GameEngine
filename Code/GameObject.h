#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "Transform.h"

class Component;

class GameObject
{
public:
	GameObject(std::string name, Transform * parent);

private:
	bool OnStart();
	bool OnUpdate(float dt);
	bool OnEnable();
	bool OnDisable();

public:
	Transform transform;

private:
	Transform * parent = nullptr;
	bool active = true;
	std::string name;
	std::vector<Component*> components;
};

#endif // !GAMEOBJECT_H_

