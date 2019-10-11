#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__



#include <vector>
#include <string>

class Component;
class Transform;
class GameObject
{
public:
	GameObject() {};

public:
	Transform* transform;
	std::string name;
	bool active = true;
	std::vector<Component*> components;
	virtual void Disable() {};
	virtual void Enable() {};
};

#endif // !GAMEOBJECT_H_

