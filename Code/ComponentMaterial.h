#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "ComponentMesh.h"
#include "Texture.h"
class ComponentMaterial : public Component
{
	CLASS_DECLARATION(ComponentMaterial)
private:
	Texture* texture = nullptr;
	ComponentMesh * component_mesh = nullptr;
public:
	ComponentMaterial(GameObject * gameobject, ComponentMesh* component_mesh );
	
	void SetTexture(Texture* texture);
	void DisableGLModes();
	void RenderTexture();
};

#endif
