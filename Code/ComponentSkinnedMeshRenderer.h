#ifndef COMPONENT_SKINNED_MESH_RENDERER_H_
#define COMPONENT_SKINNED_MESH_RENDERER_H_
#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "ComponentTransform.h"
#include <map>
#include <string>
class ResourceMesh;

class ComponentSkinnedMeshRenderer : public Component
{
	CLASS_DECLARATION(ComponentSkinnedMeshRenderer)
	ResourceMesh* mesh = nullptr;
	math::float3* vertices = nullptr;
	uint id_vertex_buffer = 0;
	void OpenBuffer();
	std::map<std::string, ComponentTransform*> bones;
	

public:
		ComponentTransform* root_object = nullptr;
		ComponentSkinnedMeshRenderer(GameObject* parent);
		void PropertiesEditor() override;
		void SetMesh(ResourceMesh*);
		void OnUpdate(float dt);
		void OnPostUpdate() override;
		void CleanUp() override;

};
#endif // !COMPONENT_SKINNED_MESH_RENDERER_H_

