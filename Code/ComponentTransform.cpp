#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject * gameobject) : Component(gameobject)
{
	name = "Transform";//TODO: Every transform comonent (and other components) don't need its own name, they can all have the same

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
