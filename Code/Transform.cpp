#include "Transform.h"

Transform::Transform(GameObject * gameobject) : Component(gameobject)
{
}

void Transform::SetParent(Transform * parent)
{
	if (parent != nullptr)
	{
		this->parent = parent;
		parent->children.push_back(this);
	}
}
