#ifndef MESH_H_
#define MESH_H_

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
class AssetMesh;

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
public:
	ComponentMesh(GameObject * gameobject);
	~ComponentMesh();
	void OnPostUpdate() override;
	void DrawVertexNormal();
	void DrawNormals();
	void PropertiesEditor() override;
	void CleanUp() override;

public:
	//One mesh can be used by more than one mesh component
	AssetMesh * mesh = nullptr;

private:

	RenderMode render_mode;
	
	//Fill
	float fill_color[4];

	//Wireframe
	float line_color[4];
	float line_width = 1.f;

	//Point
	float point_color[4];
	float point_size = 1.f;
};

#endif // !MESH_H_

