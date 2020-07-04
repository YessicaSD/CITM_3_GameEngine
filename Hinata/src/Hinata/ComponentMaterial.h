#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "ComponentMesh.h"

class ResourceTexture;
class ResourceMaterial;

class ComponentMaterial : public Component
{
	CLASS_DECLARATION(ComponentMaterial)

public:
	void CleanUp() override;
	bool SetTexture(ResourceTexture* texture);
	void DisableGLModes();
	//void RenderTexture();
	void PropertiesEditor() override;
	//void SetMeshComponent(ComponentMesh * component_mesh);
	void OnSave(JSONFile * file) override;
	void OnLoad(JSONFile * file) override;

private:
	ResourceTexture* texture = nullptr;
	ResourceMaterial* material = nullptr;

	//ComponentMesh * component_mesh = nullptr;


	ComponentMaterial(GameObject* gameobject, ComponentMesh* component_mesh);
	ComponentMaterial(GameObject* gameobject);

	friend class ComponentSkinnedMesh;
	friend class ComponentMesh;
	friend class GameObject;
};

#endif
