#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <string>

class GameObject;

class Component
{
public:
	Component(GameObject * gameobject);

	virtual void PropertiesEditor() {};
	void EnableComponent(bool value);

protected:
	virtual void OnUpdate(float dt) {};
	virtual void OnPostUpdate() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};

	bool CollapsigHeaderWithCheckbox();

public:
	//The gameobject which contains this component
	GameObject* gameobject = nullptr;

protected: 
	bool enabled = true;
	std::string name;
	
	friend class GameObject;
};


#endif // !COMPONENT_H_

