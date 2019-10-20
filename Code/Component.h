#ifndef COMPONENT_H_
#define COMPONENT_H_

class GameObject;

class Component
{
public:
	Component(GameObject * gameobject);

private:
	virtual void OnUpdate(float dt) {};
	virtual void OnPostUpdate() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};

	virtual void ShowInspector() {};

public:
	//The gameobject which contains this component
	GameObject* gameobject = nullptr;

private: 
	bool active = true;
	
	friend class GameObject;
};


#endif // !COMPONENT_H_

