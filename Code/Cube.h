#ifndef _CUBE_H_
#define _CUBE_H_

#include "MathGeoLib\include\Math\float3.h"
#define NULL 0

#define NUM_VERTECES 8

//6 faces 2 triangle for wach and each triangle is form by 3 vetexs.
#define NUM_INDICES 6*2*3

typedef unsigned int uint;


class Cube
{
	float vertices[NUM_VERTECES * 3];
	uint vetex_buf_id, indices_buf_id;
public:

	Cube(float w, float h, float l, float3	position);
	//void Draw_DirectMode();
	void Draw();
};
#endif // !_CUBE_H_
