#include "BoundingBox.h"
#include "MathGeoLib/include/Math/float4.h"
#include "glew/include/GL/glew.h"
#include <gl/GL.h>


BoundingBox::BoundingBox()
{
	SetNegativeInfinty();

	obb_color = { 1,1,0,1 };
	aabb_color = {0,1,0,1 };
	const float3 vertex = { 0,0,0 };
	const float3 size = { 1,1,1 };
	local_aabb.SetFromCenterAndSize(vertex, size);
	MultiplyByMatrix(float4x4::identity);
}

void BoundingBox::SetNegativeInfinty()
{
	local_aabb.SetNegativeInfinity();
	aabb.SetNegativeInfinity();
	obb.SetNegativeInfinity();
}

BoundingBox::~BoundingBox()
{
	CleanUp();
}

void BoundingBox::MultiplyByMatrix(float4x4 matrix)
{
	float3 corners[8];

	obb = local_aabb;
	obb.Transform(matrix);
	obb.GetCornerPoints(corners);
	obb_cube.SetVetices((float*)&corners);

	aabb.SetNegativeInfinity();
	aabb.Enclose(obb);
	aabb.GetCornerPoints(corners);
	aabb_cube.SetVetices((float*)&corners);
}

void BoundingBox::Draw()
{
	bool light = false;
	if (light = glIsEnabled(GL_LIGHTING))
	{
		glDisable(GL_LIGHTING);
	}
	glColor4f(obb_color[0], obb_color[1], obb_color[2], obb_color[3]);
	obb_cube.Draw();
	glColor4f(aabb_color[0], aabb_color[1], aabb_color[2], aabb_color[3]);

	aabb_cube.Draw();
	glColor4f(1, 1, 1, 1);
	glLineWidth(1);
	if (light)
	{
		glEnable(GL_LIGHTING);
	}
}

void BoundingBox::CleanUp()
{
	obb_cube.CleanUp();
	aabb_cube.CleanUp();
	SetNegativeInfinty();
}

void BoundingBox::SetLocalAABB(AABB local_aabb)
{
	this->local_aabb = local_aabb;
}

AABB BoundingBox::GetAABB()
{
	return aabb;
}

OBB BoundingBox::GetOBB()
{
	return obb;
}
