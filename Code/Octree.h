#ifndef OCTREE_H_
#define OCTREE_H_
#include "MathGeoLib/include/Geometry/AABB.h"
#include "CubeLine.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include <list>

#define MAX_BUCKET_SIZE 1
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
	void Erase(ComponentTransform* transform);
	

private:
	AABB limits;
	CubeLine box;
	uint division_level = 0;
	bool is_divided  = false;

	std::list<ComponentTransform*> objects;
	
	OctreeNode* childs[8];

	void CreateChildsNodes();
	void RedistributeObjects();
	void Draw();

	friend class Octree;
};

class Octree
{
public:
	void SetLimits(const AABB& box);
	void Insert(ComponentTransform* go);
	void Erase(ComponentTransform* go);
	void Clear();
	void Draw();
private:
	OctreeNode* root_node = nullptr;
};
#endif // !OCTREE_H_
