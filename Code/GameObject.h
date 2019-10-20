#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "ComponentTransform.h"
#include "ComponentMesh.h"

class Component;
class PanelProperties;

class GameObject
{
public:
	GameObject(std::string name, ComponentTransform * parent);

	bool OnStart();
	bool OnUpdate(float dt);
	bool OnPostUpdate();
	bool OnEnable();
	bool OnDisable();

	template <class ComponentClass>
	ComponentClass * CreateComponent()
	{
		ComponentClass * new_component = new ComponentClass(this);
		components.push_back(new_component);
		return new_component;
	}

	//TODO: GetComponent method (in .h)
	//template <class ComponentClass>
	//ComponentClass * GetComponent()
	//{
	//return
	//}

	const char * GetName();

public:
	ComponentTransform transform;

private:
	bool active = true;
	std::string name;
	std::vector<Component*> components;

	friend PanelProperties;
};

#endif // !GAMEOBJECT_H_

