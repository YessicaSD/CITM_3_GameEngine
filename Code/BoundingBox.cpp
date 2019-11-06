#include "BoundingBox.h"
#include "MathGeoLib/include/Math/float4.h"
#include "glew/include/GL/glew.h"
#include <gl/GL.h>
void BoundingBox::UpdateCube(float* vertices)
{
	if(vertex_buff==-1)
		glGenBuffers(1, (GLuint*) & (vertex_buff));
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_VERTICES * 3, vertices, GL_STATIC_DRAW);

	if(indices_buff == -1)
		glGenBuffers(1, (GLuint*) & (indices_buff));

	uint indices[NUM_INDICES] = { 0,2,1, 0,3,2, 1,2,5,2,6,5 ,2,3,6,3,7,6,   4,5,6,6,7,4,	0,4,3,4,7,3,	0,1,5,5,4,0 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NUM_INDICES, indices, GL_STATIC_DRAW);
}



BoundingBox::BoundingBox()
{
	const int num_indices = 6 * 4;
	uint indices[num_indices] = { 1,5,7,3, 5,4,6,7, 0,2,6,4, 1,3,2,0, 3,7,6,2, 0,4,5,1 };
	obb_cube.SetIndeces((uint*)&indices, num_indices);
	aabb_cube.SetIndeces((uint*)&indices, num_indices);
}

void BoundingBox::MultiplyByMatrix(float4x4 matrix, AABB aabb)
{
	float3 corners[8];

	obb = aabb;
	obb.Transform(matrix);
	obb.GetCornerPoints(corners);
	obb_cube.SetVetices((float*)&corners);

	this->aabb.SetNegativeInfinity();
	this->aabb.Enclose(obb);
	this->aabb.GetCornerPoints(corners);
	aabb_cube.SetVetices((float*)&corners);
}

void BoundingBox::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	obb_cube.DrawQuads();
	aabb_cube.DrawQuads();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLineWidth(1);

}

AABB BoundingBox::GetAABB()
{
	return aabb;
}
