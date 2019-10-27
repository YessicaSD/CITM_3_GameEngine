#include "ComponentTransform.h"
#include "MathGeoLib/include/Math/float4.h"
#include "imgui/imgui.h"
#include "GameObject.h"
#include "ComponentMesh.h"
CLASS_DEFINITION(Component, ComponentTransform)

ComponentTransform::ComponentTransform(GameObject * gameobject) : Component(gameobject)
{
	name = "Transform";//TODO: Every transform component (and other components) don't need its own name, they can all have the same

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

	ImGui::InputFloat3("Position", (float*)&aux_position, "%.2f");


	ImGui::InputFloat3("Rotation", (float*)&aux_rotation, "%.2f");


	ImGui::InputFloat3("Scale", (float*)&aux_scale, "%.2f");
	if (aux_position != position || aux_rotation != rotation || aux_scale != scale)
	{
		CalculGlobalMatrix(aux_position, aux_scale, aux_rotation);

	}
}

void ComponentTransform::CalculGlobalMatrix(float3 & position, float3 & scale, float3 & rotation)
{
	this->position = position; this->rotation = rotation;   this->scale = scale;

	qrotation = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);

	this->local_matrix = float4x4::FromTRS(position, qrotation, scale);

	if (parent)
	{
		this->global_matrix = this->parent->global_matrix *  this->local_matrix;
	}
	else
	{
		this->global_matrix = this->local_matrix;
	}

	aux_position = position; aux_rotation = rotation; aux_scale = scale;
	ComponentMesh* comp_mesh = gameobject->GetComponent<ComponentMesh>();
	if (comp_mesh)
	{
		comp_mesh->CalculBoindingBox();
	}

}

void ComponentTransform::CalculGlobalMatrix(float3 & position, float3 & scale, Quat & qrotation)
{
	this->position = position; 
	this->qrotation = qrotation;   
	this->scale = scale;

	this->rotation = qrotation.ToEulerXYZ();

	this->local_matrix = float4x4::FromTRS(position, qrotation, scale);

	if (parent)
	{
		this->global_matrix = this->parent->global_matrix *  this->local_matrix;
	}
	else
	{
		this->global_matrix = this->local_matrix;
	}

	aux_position = position;
	aux_rotation = rotation; 
	aux_scale = scale;
	ComponentMesh* comp_mesh = gameobject->GetComponent<ComponentMesh>();
	if (comp_mesh)
	{
		comp_mesh->CalculBoindingBox();
	}
}
