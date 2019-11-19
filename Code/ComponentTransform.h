#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "BoundingBox.h"
#include <vector>

class GameObject;

class ComponentTransform : public Component
{
	CLASS_DECLARATION(ComponentTransform)
public:
	ComponentTransform(GameObject * gameobject);
	~ComponentTransform();
	//Create 

	void SetParent(ComponentTransform * parent);
	ComponentTransform* GetParent();
	
	void OnPostUpdate() override;
	void PropertiesEditor() override;

	void SwitchedStatic();

	void SetTransform(float3& position, float3& scale, float3 & rotation);
	void SetTransform(float3& position, float3& scale, Quat & qrotation);
	void SetPosition(const float3 & position);
	void SetRotation(const float3 & euler_rotation);
	void SetRotation(const Quat & qrotation);
	void SetScale(const float3 & scale);
	void SetLocalMatrix(const float4x4& matrix);
	void SetGlobalMatrix(const float4x4& matrix);

	void SetSelected(bool state);



	bool IsSelected();
	bool Intersect(LineSegment ray);
	float3 GetZAxis();

	float3 GetYAxis();

	float3 GetPosition() const;
	Quat GetRotation() const;
	float3 GetRotationEuler() const;
	float3 GetScale() const;
	float4x4 GetGlobalMatrix() const;

	void Reset();
	void UpdateDisplayValues();
	void DeleteChildren();
	void DrawAxis();
	AABB GetAABB();

	void GetStaticObjects(std::vector<ComponentTransform*> static_objects);

	bool open_in_hierarchy = false;
	bool is_static = false;
private:
	void RecalculateMatrices();
	void UpdateChildrenMatrices();
	void UpdateVector();

private:
	float3 position = {0.f, 0.f, 0.f },
		scale = { 0.f, 0.f, 0.f },
		euler_rotation = { 0.f, 0.f, 0.f };
	Quat qrotation;

	float3 x, y, z;

	//You should modify the local matrix, the global matrix is recalculated from it and the parents' local matrix
	float4x4 local_matrix;
	float4x4 global_matrix;

	ComponentTransform * parent = nullptr;
	std::vector<ComponentTransform*> children;
	BoundingBox bounding_box;

	bool is_selected = false;

	friend class ModuleImport;
	friend class PanelHierarchy;
	friend class ModuleScene;
	
};
#endif // !TRANSFORM_H_

