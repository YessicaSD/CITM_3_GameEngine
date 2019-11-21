#ifndef OCTREE_H_
#define OCTREE_H_
#include "MathGeoLib/include/Geometry/AABB.h"
#include "CubeLine.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include <list>
#include <map>

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
	
	template<typename TYPE>
	void CollectIntersections(std::vector<ComponentTransform*>& objects, const TYPE& primitive) const;
	template<typename TYPE>
	void CollectIntersections(std::map<float, ComponentTransform*>& objects, const TYPE& primitive) const;

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

	template<typename TYPE>
	void CollectIntersections(std::vector<ComponentTransform*>& objects, const TYPE& primitive) const;
	
	//This funtion is thought to be used with raycasting
	template<typename TYPE>
	void CollectIntersections(std::map<float, ComponentTransform*>& objects, const TYPE& primitive) const;
private:
	OctreeNode* root_node = nullptr;
};

template<typename TYPE>
inline void Octree::CollectIntersections(std::vector<ComponentTransform*>& objects, const TYPE & primitive) const
{
	if (root_node  != nullptr)
	{
		root_node->CollectIntersections(objects, primitive);
	}
}

template<typename TYPE>
inline void Octree::CollectIntersections(std::map<float, ComponentTransform*>& objects, const TYPE & primitive) const
{
	if (root_node != nullptr)
	{
		root_node->CollectIntersections(objects, primitive);
	}
}

template<typename TYPE>
inline void OctreeNode::CollectIntersections(std::vector<ComponentTransform*>& objects, const TYPE & primitive) const
{
	if (primitive.Intersects(limits))
	{
		for (std::list<ComponentTransform*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->bounding_box.GetOBB()))
			{
				objects.push_back(*it);
			}
		}
		if (is_divided)
		{
			for (int i = 0; i < 8; ++i)
			{
				childs[i]->CollectIntersections(objects, primitive);
			}
		}
		
	}
}

template<typename TYPE>
inline void OctreeNode::CollectIntersections(std::map<float, ComponentTransform*>& objects, const TYPE & primitive) const
{
	if (primitive.Intersects(limits))
	{
		float hit_near, hit_far;
		for (std::list<ComponentTransform*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->bounding_box.GetOBB(), hit_near, hit_far))
				objects[hit_near] = *it;
		}
		if (is_divided)
		{
			for (uint i = 0u; i < 8; i++)
			{
				childs[i]->CollectIntersections(objects, primitive);
			}
		}
	}
}


#endif // !OCTREE_H_

