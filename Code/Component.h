#ifndef COMPONENT_H_
#define COMPONENT_H_

class GameObject;

class Component
{
public:
	Component(GameObject * gameobject);

private:
	virtual void OnUpdate(float dt) {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};

private: 
	//The gameobject which has this component
	GameObject* gameobject = nullptr;
	bool active = true;

	//Component type
	
	friend class GameObject;
};


#endif // !COMPONENT_H_

