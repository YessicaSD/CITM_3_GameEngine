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
	void SetTransform(float3& position, float3& scale, float3 & rotation);
	void SetTransform(float3& position, float3& scale, Quat & qrotation);
	void SetTransform(const float4x4 & local_matrix);
	void SetPosition(const float3 & position);
	void SetRotation(const float3 & euler_rotation);
	void SetRotation(const Quat & qrotation);
	void SetScale(const float3 & scale);


	float3 GetPosition() const;
	Quat GetRotation() const;
	float3 GetRotationEuler() const;
	float3 GetScale() const;
	float4x4 GetGlobalMatrix() const;

	void Reset();
	void UpdateDisplayValues();
	void DeleteChildren();

private:
	void RecalculateMatrices();
	void UpdateChildrenMatrices();

private:
	float3 position = {0.f, 0.f, 0.f },
		scale = { 0.f, 0.f, 0.f },
		euler_rotation = { 0.f, 0.f, 0.f };
	Quat qrotation;

	//You should modify the local matrix, the global matrix is recalculated from it and the parents' local matrix
	float4x4 local_matrix;
	float4x4 global_matrix;

	ComponentTransform * parent = nullptr;
	std::vector<ComponentTransform*> children;

	friend class PanelHierarchy;
	friend class ModuleScene;
};
#endif // !TRANSFORM_H_

