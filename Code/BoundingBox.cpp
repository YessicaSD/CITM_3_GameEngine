#include "BoundingBox.h"
#include "MathGeoLib/include/Math/float4.h"
void BoundingBox::UpdateCube()
{
	float3 position = aabb.CenterPoint();
	float width = aabb.maxPoint.x - aabb.minPoint.x;
	float height = aabb.maxPoint.y - aabb.minPoint.y;
	float depth = aabb.maxPoint.z - aabb.minPoint.z;

	cube.Set(width, height, depth, position);
}

void BoundingBox::CalculateBoundingBoxFromVertex(float3 * vertices, int num_vertices)
{
	aabb.Enclose(vertices, num_vertices);
	UpdateCube();

}

void BoundingBox::MultiplyByMatrix(float4x4 matrix)
{
	float4 tmp_maxPoint = { aabb.maxPoint.x,  aabb.maxPoint.y, aabb.maxPoint.z, 1 };
	float4 tmp_minPoint = { aabb.minPoint.x,  aabb.minPoint.y, aabb.minPoint.z, 1 };

	tmp_maxPoint =  matrix.Mul(tmp_maxPoint);
	tmp_minPoint =  matrix.Mul(tmp_minPoint);
	aabb.minPoint = { tmp_minPoint.x, tmp_minPoint.y, tmp_minPoint.z };
	aabb.maxPoint = { tmp_maxPoint.x, tmp_maxPoint.y, tmp_maxPoint.z };
	UpdateCube();
}

void BoundingBox::Draw(const float * matrix)
{
	cube.Draw(matrix);
}

AABB BoundingBox::GetAABB()
{
	return aabb;
}
