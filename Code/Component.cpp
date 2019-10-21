#include "Component.h"

Component::Component(GameObject * gameobject):
	gameobject(gameobject)
{
}

void Component::SetActive(bool value)
{
	if (value)
	{
		enabled = true;
		OnEnable();
	}
	else
	{
		enabled = false;
		OnDisable();
	}
}
