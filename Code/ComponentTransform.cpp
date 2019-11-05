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
	bool position_changed = false,
		rotation_changed = false,
		scale_changed = false;
	if (ImGui::InputFloat3("Position", (float*)&position, "%.2f"))
	{
		position_changed = true;
	}
	if (ImGui::InputFloat3("Rotation", (float*)&euler_rotation, "%.2f"))
	{
		rotation_changed = true;
	}
	if (ImGui::InputFloat3("Scale", (float*)&scale, "%.2f"))
	{
		scale_changed = true;
	}
	if (position_changed || rotation_changed|| scale_changed)
	{
		SetTransform(position, scale, euler_rotation * DEGTORAD);
	}
}

void ComponentTransform::SetTransform(float3 & position, float3 & scale, float3 & rotation)
{
	this->position = position; 
	this->euler_rotation = rotation; 
	this->scale = scale;

	qrotation = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);

	this->local_matrix = float4x4::FromTRS(position, qrotation, scale);

	if (parent != nullptr)
	{
		global_matrix = parent->global_matrix *  local_matrix;
	}
	else
	{
		global_matrix = local_matrix;
	}

	ComponentMesh* comp_mesh = gameobject->GetComponent<ComponentMesh>();
	if (comp_mesh != nullptr)
	{
		comp_mesh->CalculateBoundingBox();
	}

	if (children.size() > 0)
	{
		for (std::vector<ComponentTransform*>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			(*iter)->global_matrix = global_matrix * (*iter)->local_matrix;
			(*iter)->UpdateDisplayValues();
		}
	}
}

void ComponentTransform::SetTransform(float3 & position, float3 & scale, Quat & qrotation)
{
	this->position = position; 
	this->qrotation = qrotation;   
	this->scale = scale;

	this->euler_rotation = qrotation.ToEulerXYZ();
	this->euler_rotation *= RADTODEG;

	this->local_matrix = float4x4::FromTRS(position, qrotation, scale);

	if (parent != nullptr)
	{
		global_matrix = parent->global_matrix *  local_matrix;
	}
	else
	{
		global_matrix = local_matrix;
	}

	ComponentMesh* comp_mesh = gameobject->GetComponent<ComponentMesh>();
	if (comp_mesh != nullptr)
	{
		comp_mesh->CalculateBoundingBox();
	}

	if (children.size() > 0)
	{
		for (std::vector<ComponentTransform*>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			(*iter)->global_matrix = global_matrix * (*iter)->local_matrix;
			(*iter)->UpdateDisplayValues();
		}
	}
}

void ComponentTransform::SetPosition(float3 &position)
{
	this->position = position;

	local_matrix = float4x4::FromTRS(position, qrotation, scale);

	if (parent != nullptr)
	{
		global_matrix = parent->global_matrix *  local_matrix;
	}
	else
	{
		global_matrix = local_matrix;
	}

	ComponentMesh* comp_mesh = gameobject->GetComponent<ComponentMesh>();
	if (comp_mesh != nullptr)
	{
		comp_mesh->CalculateBoundingBox();
	}

	if (children.size() > 0)
	{
		for (std::vector<ComponentTransform*>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			(*iter)->global_matrix = global_matrix * (*iter)->local_matrix;
			(*iter)->UpdateDisplayValues();
		}
	}
}

float3 ComponentTransform::GetPosition() const
{
	return position;
}

Quat ComponentTransform::GetRotation() const
{
	return qrotation;
}

float3 ComponentTransform::GetRotationEuler() const
{
	return euler_rotation;
}

float3 ComponentTransform::GetScale() const
{
	return scale;
}

float4x4 ComponentTransform::GetGlobalMatrix() const
{
	return global_matrix;
}

void ComponentTransform::Reset()
{
	SetTransform(float3(0, 0, 0), float3(1, 1, 1), float3(0, 0, 0));
}

void ComponentTransform::UpdateDisplayValues()
{
	global_matrix.Decompose(position, qrotation, scale);
	euler_rotation = qrotation.ToEulerXYZ();
}

void ComponentTransform::DeleteChildren()
{
	if (gameobject->transform->children.size() > 0)
	{
		for (std::vector<ComponentTransform*>::iterator iter = gameobject->transform->children.begin(); iter != gameobject->transform->children.end(); ++iter)
		{
			(*iter)->DeleteChildren();
			delete (*iter);
		}
		gameobject->transform->children.clear();
	}
}
