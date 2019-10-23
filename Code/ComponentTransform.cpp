#include "ComponentTransform.h"
#include "MathGeoLib/include/Math/float4.h"

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
	global_matrix.Decompose(position, rotation, scale);

}
