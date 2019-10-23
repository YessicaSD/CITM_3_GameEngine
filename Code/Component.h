#ifndef COMPONENT_H_
#define COMPONENT_H_

class GameObject;

class Component
{
public:
	Component(GameObject * gameobject);

	virtual void PropertiesEditor() {};
	void SetActive(bool value);

private:
	virtual void OnUpdate(float dt) {};
	virtual void OnPostUpdate() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void CleanUp() {};

public:
	//The gameobject which contains this component
	GameObject* gameobject = nullptr;

private: 
	bool enabled = true;
	
	friend class GameObject;
};


#endif // !COMPONENT_H_

