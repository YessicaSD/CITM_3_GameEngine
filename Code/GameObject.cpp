#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"

GameObject::GameObject(std::string name, ComponentTransform * parent):
	transform(this),
	name(name)
{
	transform.SetParent(parent);
	components.push_back(&transform);
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
		if ((*iter)->enabled)
		{
			(*iter)->OnUpdate(dt);
		}
	}
	return true;
}

bool GameObject::OnPostUpdate()
{
	for (std::vector<Component*>::iterator iter = components.begin();
		iter != components.end();
		++iter)
	{
		if ((*iter)->enabled)
		{
			(*iter)->OnPostUpdate();
		}
	}
	return true;
}

bool GameObject::OnActivate()
{
	if (!active) 
	{
		for (std::vector<Component*>::iterator iter = components.begin();
			iter != components.end();
			++iter)
		{
			(*iter)->OnEnable();
		}
		active = true;
	}
	return true;
}

bool GameObject::OnDeactivate()
{
	if (active) 
	{
		for (std::vector<Component*>::iterator iter = components.begin();
			iter != components.end();
			++iter)
		{
			(*iter)->OnDisable();
		}
		active = false;
	}
	return true;
}
