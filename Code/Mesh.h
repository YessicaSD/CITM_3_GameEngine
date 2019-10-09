#ifndef MESH_H_
#define MESH_H_

typedef unsigned int uint;
#include "MathGeoLib/include/Math/float3.h"
#include "Assimp/include/vector3.h"
struct Mesh {

	//Data ========================
	uint id_indice = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;


	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	aiVector3D* normals;
	uint numFaces = 0;
	float3* faces_normals;
	float3* face_middle_point;

	bool Load(uint num_vertices, float* mVetices);

	void Draw();

	void DrawVertexNormal();

	void DrawNormals();

	

};

#endif // !MESH_H_

