#ifndef CUBEQUAD_H_
#define CUBEQUAD_H_

#include "MathGeoLib\include\Math\float3.h"

#define NULL 0

#define NUM_VERTICES 8

//A cube has 6 faces
//Each face has 2 triangles
//Each triangle is form by 3 vetices
#define NUM_INDICES 8*4

typedef unsigned int uint;


class CubeQuad
{
	float vertices[NUM_VERTICES * 3];
	uint vetex_buf_id, indices_buf_id;
private:
	void OpenBuffers();
	void SetIndices();
public:

	CubeQuad(float width, float height, float depth, float3	position);
	CubeQuad();

	void Draw();
	void Set(float width, float height, float depth, float3 position);
	void SetVetices(float* vertices);
	


};


#endif // !CUBEQUAD_H_

