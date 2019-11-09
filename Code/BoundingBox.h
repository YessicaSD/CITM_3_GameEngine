#ifndef  BOUNDING_BOX_H_
#define  BOUNDING_BOX_H_
#include "CubeLine.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

class BoundingBox
{

private:
	CubeLine obb_cube, aabb_cube;
	AABB local_aabb, aabb;
	OBB	 obb;
	
public:

	float4 aabb_color, obb_color;
	BoundingBox();
	void MultiplyByMatrix(float4x4 matrix);
	void Draw();
	void SetLocalAABB(AABB local_aabb);
	AABB GetAABB();

};


#endif // ! BOUNDING_BOX_H_
