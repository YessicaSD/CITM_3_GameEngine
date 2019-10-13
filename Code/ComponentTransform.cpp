#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject * gameobject) : Component(gameobject)
{
	//TODO: This is for testing purposes, remove when done
	global_matrix = local_matrix = local_matrix.identity;

}

void ComponentTransform::SetParent(ComponentTransform * parent)
{
	if (parent != nullptr)
	{
		this->parent = parent;
		parent->children.push_back(this);
	}
}
