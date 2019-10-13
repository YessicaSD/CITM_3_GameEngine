#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"

GameObject::GameObject(std::string name, ComponentTransform * parent):
	transform(this),
	name(name)
{
	transform.SetParent(parent);
}

const char * GameObject::GetName()
{
	return name.c_str();
}

bool GameObject::OnStart()
{

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

bool GameObject::OnPostUpdate()
{
	for (std::vector<Component*>::iterator iter = components.begin();
		iter != components.end();
		++iter)
	{
		(*iter)->OnPostUpdate();
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
