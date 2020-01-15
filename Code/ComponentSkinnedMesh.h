#ifndef __COMPONENT_SKINNED_MESH_H__
#define __COMPONENT_SKINNED_MESH_H__

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "ComponentTransform.h"
#include <map>
#include <string>

class ResourceMesh;
class ComponentMaterial;

//Component that renders a mesh that's being deformed by some bones.
//The bones are being moved by a ComponentAnimator.

class ComponentSkinnedMesh : public Component
{
	CLASS_DECLARATION(ComponentSkinnedMesh)
	ResourceMesh* mesh = nullptr;
	math::float3* vertices = nullptr;
	float3 *vertex_normals = nullptr;
	uint id_vertex_buffer = 0;
	uint id_vertex_normals_buffer = 0;
	void OpenBuffer();
	std::map<std::string, ComponentTransform*> bones;

public:
	ComponentTransform * root_object = nullptr;
	ComponentMaterial * material = nullptr;
	ComponentSkinnedMesh(GameObject* parent);
	void PropertiesEditor() override;
	void SetMesh(ResourceMesh*);
	void OnUpdate(float dt);
	void OnPostUpdate() override;
	void CleanUp() override;

};
#endif // !COMPONENT_SKINNED_MESH_RENDERER_H_

