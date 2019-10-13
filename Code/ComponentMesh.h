#ifndef MESH_H_
#define MESH_H_

#include "Component.h"

class AssetMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject * gameobject);
	void OnPostUpdate() override;
	void DrawVertexNormal();
	void DrawNormals();

public:
	//One mesh can be used by more than one mesh component
	AssetMesh * mesh = nullptr;
};

#endif // !MESH_H_

