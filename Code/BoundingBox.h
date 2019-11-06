#ifndef  BOUNDING_BOX_H_
#define  BOUNDING_BOX_H_
#include "Cube.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
class BoundingBox
{
private:
	int vertex_buff = -1, indices_buff = -1;
	Cube obb_cube;
	Cube aabb_cube;
	AABB aabb;
	OBB	 obb;
	void UpdateCube(float* vertices);
public:
	BoundingBox();
	void MultiplyByMatrix(float4x4 matrix, AABB aabb);
	void Draw();
	AABB GetAABB();

};


#endif // ! BOUNDING_BOX_H_
