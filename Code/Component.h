#ifndef COMPONENT_H_
#define COMPONENT_H_

class GameObject;

class Component
{
public:
	Component(GameObject * gameobject);

	virtual void PropertiesEditor() {};
	void SetActive(bool value);

protected:
	virtual void OnUpdate(float dt) {};
	virtual void OnPostUpdate() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};


public:
	//The gameobject which contains this component
	GameObject* gameobject = nullptr;

protected: 
	bool enabled = true;
	
	friend class GameObject;
};


#endif // !COMPONENT_H_

