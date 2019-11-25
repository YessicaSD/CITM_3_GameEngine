#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "ComponentMesh.h"

class ResourceTexture;

class ComponentMaterial : public Component
{
	CLASS_DECLARATION(ComponentMaterial)

private:
	ComponentMaterial(GameObject * gameobject, ComponentMesh* component_mesh );
	ComponentMaterial(GameObject * gameobject);

public:
	void CleanUp() override;
	bool SetTexture(ResourceTexture* texture);
	void DisableGLModes();
	void RenderTexture();
	void PropertiesEditor() override;
	void SetMeshComponent(ComponentMesh * component_mesh);
	void OnSave(JSONFile * file) override;
	void OnLoad(JSONFile * file) override;

private:
	ResourceTexture* texture = nullptr;
	ComponentMesh * component_mesh = nullptr;

	friend class GameObject;
};

#endif
