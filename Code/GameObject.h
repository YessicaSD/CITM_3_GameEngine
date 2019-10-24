#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "ComponentTransform.h"
#include "ComponentMesh.h"

class Component;
class PanelProperties;
class PanelHierarchy;

class GameObject
{
public:
	GameObject(std::string name, ComponentTransform * parent);
	~GameObject();
	bool OnStart();
	bool OnUpdate(float dt);
	bool OnPostUpdate();
	void SetActive(bool value);

	template <class ComponentClass>
	ComponentClass * CreateComponent()
	{
		ComponentClass * new_component = new ComponentClass(this);
		components.push_back(new_component);
		return new_component;
	}

	template< class ComponentClass >
	ComponentClass &        GetComponent()
	{
		for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
		{
			if ((*iter)->IsClassType(ComponentType::Type))
				return *static_cast<ComponentType *>(component.get());
		}
		return *std::unique_ptr< ComponentType >(nullptr);
	}
	//TODO: GetComponent method (in .h)
	//template <class ComponentClass>
	//ComponentClass * GetComponent()
	//{
	//return
	//}

	const char * GetName();

private:
	bool ActivateComponents();
	bool DeactivateComponents();

public:
	ComponentTransform* transform;

private:
	bool active = true;
	std::string name;
	std::vector<Component*> components;

	friend PanelProperties;
	friend PanelHierarchy;
	friend class ModuleScene;
};

#endif // !GAMEOBJECT_H_

