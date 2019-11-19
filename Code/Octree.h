#ifndef OCTREE_H_
#define OCTREE_H_
#include "MathGeoLib/include/Geometry/AABB.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include <list>

#define MAX_BUCKET_SIZE 5
#define MAX_SUBDIVISION 15

/*
		----------- MaxPoint
		| NW | NE |
		|---------|
		| SW | SE |
		-----------
MinPoint
*/

enum OctreeIndix
{
	TOP_NW,
	TOP_NE,
	TOP_SW,
	TOP_SE,
	BOTTOM_NW,
	BOTTOM_NE,
	BOTTOM_SW,
	BOTTOM_SE,
};

class OctreeNode
{
public:
	OctreeNode(const AABB& limits);
	void Insert(ComponentTransform* transform);


private:
	AABB limits;

	uint division_level = 0;
	bool is_divided  = false;

	std::list<ComponentTransform*> objects;
	OctreeNode* parent = nullptr;
	OctreeNode* childs[8];

	void CreateChildsNodes();
	void RedistributeObjects();

	friend class Octree;
};

class Octree
{
public:
	void SetLimits(const AABB& box);
	void Insert(ComponentTransform* go);
	void Clear();

private:
	OctreeNode* root_node = nullptr;
};
#endif // !OCTREE_H_
