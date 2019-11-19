#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"

GameObject::GameObject(std::string name, ComponentTransform * parent):
	name(name)
{
	transform = new ComponentTransform(this);
	transform->SetParent(parent);
	components.push_back(transform);
}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); iter++)
	{
		Component* aux = (*iter);
		if (aux)
		{
			delete aux;
		}
	}
	components.clear();
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

bool GameObject::ActivateComponents()
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

bool GameObject::DeactivateComponents()
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

void GameObject::SetActive(bool value)
{
	active = value;
}


