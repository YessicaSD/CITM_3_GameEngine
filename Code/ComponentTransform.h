#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/Quat.h"
#include <vector>

class GameObject;

class ComponentTransform : public Component
{
	CLASS_DECLARATION(ComponentTransform)
public:
	ComponentTransform(GameObject * gameobject);
	//Create 

	void SetParent(ComponentTransform * parent);
	//void SetChildren(std::vector<Transform*> children);
	void UpdatePos();
	void PropertiesEditor() override;
	void CalculGlobalMatrix(float3& position, float3& scale, float3 & qrotation);
public:
	float3 position = {0,0,0}, scale = { 0,0,0 }, rotation = { 0,0,0 };
	Quat qrotation;

	//You should modify the local matrix, the global matrix is recalculated from it and the parents' local matrix
	float4x4 local_matrix;

	float4x4 global_matrix;

private:
	ComponentTransform * parent = nullptr;
	std::vector<ComponentTransform*> children;
	
	friend class PanelHierarchy;
	friend class ModuleScene;
};
#endif // !TRANSFORM_H_

