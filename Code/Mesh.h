#ifndef MESH_H_
#define MESH_H_

typedef unsigned int uint;

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "Assimp/include/vector3.h"

class Mesh : public Component
{
public:
	Mesh(GameObject * gameobject);
	bool LoadMesh(uint num_vertices, float* mVetices);
	void Draw();
	void DrawVertexNormal();
	void DrawNormals();

public:
	uint id_indice = 0u; // index in VRAM
	uint num_indices = 0u;
	uint* indices = nullptr;

	uint id_vertex = 0u; // unique vertex in VRAM
	uint num_vertices = 0u;
	float* vertices = nullptr;

	aiVector3D* normals = nullptr;
	uint numFaces = 0u;
	float3* faces_normals = nullptr;
	float3* face_middle_point = nullptr;
	//TODO: We don't need float 3 pointer, we can do it with float, as the vertices
};

#endif // !MESH_H_

