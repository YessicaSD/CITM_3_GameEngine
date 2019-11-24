#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "JSONFile.h"

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
	void OnSave(JSONFile*);
	void OnLoad(JSONFile*);

	template <class ComponentClass>
	ComponentClass * CreateComponent()
	{
		ComponentClass * new_component = new ComponentClass(this);
		components.push_back(new_component);
		return new_component;
	}

	template< class ComponentClass >
	ComponentClass* GetComponent()
	{
		for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
		{
			if ((*iter) && (*iter)->IsClassType(ComponentClass::type))
			{
				return (ComponentClass*)(*iter);

			}
		}
		return nullptr;
	}
	template< class ComponentType >
	bool RemoveComponent()
	{
		if (components.empty())
		{
			return false;
		}

		for (std::vector<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter)
		{
			if ((*iter)->IsClassType(ComponentType::type))
			{
				(*iter)->CleanUp();
				delete (*iter);
				components.erase(iter);
				return true;
			}
		}
	
		return false;
	}

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

	friend class ComponentTransform;
	friend PanelProperties;
	friend PanelHierarchy;
	friend class ModuleScene;
	friend class ComponentMesh;
};

#endif // !GAMEOBJECT_H_

