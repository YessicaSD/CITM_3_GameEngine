#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "Transform.h"
#include "Mesh.h"

class Component;

class GameObject
{
public:
	GameObject(std::string name, Transform * parent);

	template <class ComponentClass>
	ComponentClass * CreateComponent()
	{
		ComponentClass * new_component = new ComponentClass(this);
		components.push_back(new_component);
		return new_component;
	}

	//TODO: Get component method

private:
	bool OnStart();
	bool OnUpdate(float dt);
	bool OnEnable();
	bool OnDisable();

public:
	Transform transform;

private:

	Transform * parent = nullptr;
	std::vector<Transform*> children;

	bool active = true;
	std::string name;
	std::vector<Component*> components;
};

#endif // !GAMEOBJECT_H_

