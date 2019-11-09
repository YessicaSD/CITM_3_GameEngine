#ifndef MESH_H_
#define MESH_H_

#include "Component.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "BoundingBox.h"

class AssetMesh;
class ComponentMaterial;

struct RenderMode
{
	bool fill = true;
	bool wireframe = false;
	bool point = false;
	bool vertex_normals = false;
	bool face_normals = false;
};

class ComponentMesh : public Component
{
	CLASS_DECLARATION(ComponentMesh)
public:
	ComponentMesh(GameObject *gameobject);
	~ComponentMesh();
	void OnPostUpdate() override;
	void DrawVertexNormal();
	void DrawNormals();
	void PropertiesEditor() override;
	void CleanUp() override;	

private:
	AssetMesh *mesh = nullptr;

	ComponentMaterial *material = nullptr;
	RenderMode render_mode;

	//Fill
	float fill_color[4];

	//Wireframe
	float line_color[4];
	float line_width = 1.f;

	//Point
	float point_color[4];
	float point_size = 1.f;
	friend class ModuleImport;
	friend class ComponentMaterial;
};

#endif // !MESH_H_
