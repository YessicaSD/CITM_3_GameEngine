#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "GameObject.h"
class Component
{
	bool active = true;
	GameObject* myObject;
	virtual void Disable() {};
	virtual void Enable() {};
};


#endif // !COMPONENT_H_

