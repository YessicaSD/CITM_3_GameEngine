#ifndef MESH_H_
#define MESH_H_

#include "Component.h"

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
	void OnPostUpdate() override;
	void DrawVertexNormal();
	void DrawNormals();
	void ShowInspector();

public:
	//One mesh can be used by more than one mesh component
	AssetMesh * mesh = nullptr;

private:
	RenderMode render_mode;
};

#endif // !MESH_H_

