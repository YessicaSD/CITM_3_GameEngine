#ifndef  BOUNDING_BOX_H_
#define  BOUNDING_BOX_H_
#include "CubeQuad.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

class BoundingBox
{

private:
	CubeQuad obb_cube, aabb_cube;
	AABB aabb;
	OBB	 obb;
	
public:

	float4 aabb_color, obb_color;
	BoundingBox();
	void MultiplyByMatrix(float4x4 matrix, AABB aabb);
	void Draw();
	AABB GetAABB();

};


#endif // ! BOUNDING_BOX_H_
