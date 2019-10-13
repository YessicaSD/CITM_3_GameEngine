#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>

class GameObject;

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject * gameobject);
	//Create 

	void SetParent(ComponentTransform * parent);
	//void SetChildren(std::vector<Transform*> children);

public:
	float3 position;
	float3 rotation;

private:
	ComponentTransform * parent = nullptr;
	std::vector<ComponentTransform*> children;

	friend class PanelHierarchy;
	friend class ModuleScene;
};
#endif // !TRANSFORM_H_

