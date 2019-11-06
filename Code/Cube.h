#ifndef _CUBE_H_
#define _CUBE_H_

#include "MathGeoLib\include\Math\float3.h"

#define NULL 0

#define NUM_VERTICES 8

//A cube has 6 faces
//Each face has 2 triangles
//Each triangle is form by 3 vetices
#define NUM_INDICES 6*2*3

typedef unsigned int uint;


class Cube
{
	float vertices[NUM_VERTICES * 3];
	uint vetex_buf_id, indices_buf_id;
	
public:

	Cube(float width, float height, float depth, float3	position);
	Cube();


	//void Draw_DirectMode();
	void Draw();
	void DrawQuads();
	void Set(float width, float height, float depth, float3 position);
	void SetVetices(float* vertices);
	void SetIndeces(uint* indices,const int& num_indices);
	//Only call ones
	void OpenBuffers();


};
#endif // !_CUBE_H_
