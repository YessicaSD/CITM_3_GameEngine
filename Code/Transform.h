#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>

class GameObject;

class Transform : public Component
{
public:
	Transform(GameObject * gameobject);
	//Create 

	void SetParent(Transform * parent);
	//void SetChildren(std::vector<Transform*> children);

public:
	float3 position;
	float3 rotation;

private:
	Transform * parent = nullptr;
	std::vector<Transform*> children;

	friend class PanelHierarchy;
};
#endif // !TRANSFORM_H_

