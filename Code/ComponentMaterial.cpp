#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject * gameobject) : Component(gameobject)
{
	name = "Material";
}

void ComponentMaterial::OnPostUpdate()
{
	if (CollapsigHeaderWithCheckbox())
	{
	}
}
