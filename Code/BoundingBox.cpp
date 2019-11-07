#include "BoundingBox.h"
#include "MathGeoLib/include/Math/float4.h"
#include "glew/include/GL/glew.h"
#include <gl/GL.h>


BoundingBox::BoundingBox()
{
	obb_color = { 1,1,0,1 };
	aabb_color = {0,1,0,1 };
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

	glColor4f(obb_color[0], obb_color[1], obb_color[2], obb_color[3]);
	obb_cube.Draw();
	glColor4f(aabb_color[0], aabb_color[1], aabb_color[2], aabb_color[3]);

	aabb_cube.Draw();
	glColor4f(1, 1, 1, 1);
	glLineWidth(1);

}

AABB BoundingBox::GetAABB()
{
	return aabb;
}
