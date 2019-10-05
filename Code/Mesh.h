#ifndef MESH_H_
#define MESH_H_

typedef unsigned int uint;

struct Mesh {

	//Data ========================
	uint id_indice = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;


	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;



	bool Load(uint num_vertices, float* mVetices);

	void Draw();

	

};

#endif // !MESH_H_

