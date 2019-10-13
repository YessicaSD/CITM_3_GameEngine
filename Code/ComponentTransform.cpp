#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject * gameobject) : Component(gameobject)
{
}

void ComponentTransform::SetParent(ComponentTransform * parent)
{
	if (parent != nullptr)
	{
		this->parent = parent;
		parent->children.push_back(this);
	}
}
