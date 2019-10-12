#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(std::string name, Transform * parent):
	name(name),
	parent(parent)
{
}

bool GameObject::OnStart()
{
	for (std::vector<Component*>::iterator iter = components.begin();
		iter != components.end();
		++iter)
	{
		(*iter)->OnStart();
	}
	return true;
}

bool GameObject::OnUpdate(float dt)
{
	for (std::vector<Component*>::iterator iter = components.begin();
		iter != components.end();
		++iter)
	{
		(*iter)->OnUpdate(dt);
	}
	return true;
}

bool GameObject::OnEnable()
{
	for (std::vector<Component*>::iterator iter = components.begin();
		iter != components.end();
		++iter)
	{
		(*iter)->OnDisable();
	}
	return true;
}

bool GameObject::OnDisable()
{
	for (std::vector<Component*>::iterator iter = components.begin();
		iter != components.end();
		++iter)
	{
		(*iter)->OnEnable();
	}
	return true;
}
