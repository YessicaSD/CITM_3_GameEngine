#include "ComponentTransform.h"
#include "MathGeoLib/include/Math/float4.h"
#include "imgui/imgui.h"
CLASS_DEFINITION(Component, ComponentTransform)

ComponentTransform::ComponentTransform(GameObject * gameobject) : Component(gameobject)
{
	name = "Transform";//TODO: Every transform comonent (and other components) don't need its own name, they can all have the same

	//TODO: This is for testing purposes, remove when done
	global_matrix = local_matrix = local_matrix.identity;
	UpdatePos();
}

void ComponentTransform::SetParent(ComponentTransform * parent)
{
	if (parent != nullptr)
	{
		this->parent = parent;
		parent->children.push_back(this);
	}
}

void ComponentTransform::UpdatePos()
{
	float4 homogeneous_vec_pos = { 0,0,0,1 };
	homogeneous_vec_pos = homogeneous_vec_pos * global_matrix;
	position = { homogeneous_vec_pos.x, homogeneous_vec_pos.y,homogeneous_vec_pos.z };
	global_matrix.Decompose(position, qrotation, scale);

}

void ComponentTransform::PropertiesEditor()
{
	static float3 aux_position = position;
	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::InputFloat3("", (float*)&aux_position, "%.2f");

	static float3 aux_rotation = rotation;
	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::InputFloat3("", (float*)&aux_rotation, "%.2f");
	
	static float3 aux_scale = scale;
	ImGui::Text("Scale");
	ImGui::SameLine();
	ImGui::InputFloat3("", (float*)&aux_scale, "%.2f");
}
