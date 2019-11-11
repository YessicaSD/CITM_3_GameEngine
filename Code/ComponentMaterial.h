#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceTexture.h"
class ComponentMaterial : public Component
{
	CLASS_DECLARATION(ComponentMaterial)
private:
	ResourceTexture* texture = nullptr;
	ComponentMesh * component_mesh = nullptr;
public:
	ComponentMaterial(GameObject * gameobject, ComponentMesh* component_mesh );
	
	void SetTexture(AssetTexture* texture);
	void DisableGLModes();
	void RenderTexture();
	void PropertiesEditor() override;

};

#endif
