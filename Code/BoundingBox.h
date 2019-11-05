#ifndef  BOUNDING_BOX_H_
#define  BOUNDING_BOX_H_
#include "Cube.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Geometry/AABB.h"

class BoundingBox
{
private:
	Cube cube;
	AABB aabb;
	void UpdateCube();
public:
	void CalculateBoundingBoxFromVertex(float3* vertices, int num_vertices);
	void MultiplyByMatrix(float4x4 matrix);
	void Draw(const float *);
	AABB GetAABB();

};


#endif // ! BOUNDING_BOX_H_
