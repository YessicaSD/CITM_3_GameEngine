#include "ComponentTransform.h"
#include "MathGeoLib/include/Math/float4.h"
#include "imgui/imgui.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Globals.h"

CLASS_DEFINITION(Component, ComponentTransform)

ComponentTransform::ComponentTransform(GameObject * gameobject) : Component(gameobject)
{
	name = "Transform";//TODO: Every transform component (and other components) don't need its own name, they can all have the same

	//TODO: This is for testing purposes, remove when done
	global_matrix = local_matrix = local_matrix.identity;
	UpdatePos();

	aux_position = position;
	aux_rotation = { 0,0,0 };
	aux_scale = { 1,1,1 };
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
		CalculGlobalMatrix(aux_position, aux_scale, float3(DEGTORAD * aux_rotation.x, DEGTORAD * aux_rotation.y, DEGTORAD* aux_rotation.z));
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

	ComponentMesh* comp_mesh = gameobject->GetComponent<ComponentMesh>();
	if (comp_mesh)
	{
		comp_mesh->CalculBoindingBox();
	}
	if (children.size() > 0)
	{
		for (std::vector<ComponentTransform*>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			(*iter)->global_matrix = this->global_matrix * (*iter)->local_matrix;
			(*iter)->CalculPRSWithMatrix();
		}
	}
}

void ComponentTransform::CalculGlobalMatrix(float3 & position, float3 & scale, Quat & qrotation)
{
	this->position = position; 
	this->qrotation = qrotation;   
	this->scale = scale;

	this->rotation = qrotation.ToEulerXYZ();
	this->rotation  *= RADTODEG;

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

void ComponentTransform::CalculPRSWithMatrix()
{
	global_matrix.Decompose(position, qrotation, scale);
	rotation = qrotation.ToEulerXYZ();

	aux_position = position;
	aux_rotation = rotation;
	aux_scale = scale;
}
