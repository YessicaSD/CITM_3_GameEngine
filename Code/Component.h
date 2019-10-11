#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "GameObject.h"
class Component
{
private: 
	GameObject* myObject = nullptr;
public:
	bool active = true;
	virtual void Disable() {};
	virtual void Enable() {};
};


#endif // !COMPONENT_H_

