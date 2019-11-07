#include "BoundingBox.h"
#include "MathGeoLib/include/Math/float4.h"
#include "glew/include/GL/glew.h"
#include <gl/GL.h>


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
	glColor3f(0, 0, 1);
	obb_cube.Draw();
	glColor3f(0, 1, 0);
	aabb_cube.Draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1);

}

AABB BoundingBox::GetAABB()
{
	return aabb;
}
