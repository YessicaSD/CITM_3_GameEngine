#include "Octree.h"
#include "Globals.h"
#include "mmgr/mmgr.h"

OctreeNode::OctreeNode(const AABB & limits): limits(limits)
{
	
}

void OctreeNode::Insert(ComponentTransform * transform)
{
	if (!is_divided &&
		objects.size() < MAX_BUCKET_SIZE)
	{
		objects.push_back(transform);
	}
	else
	{
		if (!is_divided)
		{
			CreateChildsNodes();
		}
		objects.push_back(transform);

	}
}

void OctreeNode::CreateChildsNodes()
{
	float3 size(limits.Size());
	float3 new_size(size.x*0.5f, size.y*0.5f, size.z*0.5f); 
	
	float3 center(limits.CenterPoint());
	float3 new_center(center);
	AABB new_box;

	//TOP ====================
		// NorthEast
		new_center.x = center.x + size.x * 0.25f;
		new_center.y = center.y + size.y * 0.25f;
		new_center.z = center.z + size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		childs[TOP_NE] = new OctreeNode(new_box);

		// NorthWest
		new_center.x = center.x - size.x * 0.25f;
		new_center.y = center.y + size.y * 0.25f;
		new_center.z = center.z + size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		childs[TOP_NW] = new OctreeNode(new_box);

		// SouthEast
		new_center.x = center.x + size.x * 0.25f;
		new_center.y = center.y + size.y * 0.25f;
		new_center.z = center.z - size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		childs[TOP_SE] = new OctreeNode(new_box);

		// SouthWest
		new_center.x = center.x - size.x * 0.25f;
		new_center.y = center.y + size.y * 0.25f;
		new_center.z = center.z - size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		childs[TOP_SW] = new OctreeNode(new_box);
	
	//BOTTOM =================
		// NorthEast
		new_center.x = center.x + size.x * 0.25f;
		new_center.y = center.y - size.y * 0.25f;
		new_center.z = center.z + size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		childs[BOTTOM_NE] = new OctreeNode(new_box);

		// NorthWest
		new_center.x = center.x - size.x * 0.25f;
		new_center.y = center.y - size.y * 0.25f;
		new_center.z = center.z + size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		childs[BOTTOM_NW] = new OctreeNode(new_box);

		// SouthEast
		new_center.x = center.x + size.x * 0.25f;
		new_center.y = center.y - size.y * 0.25f;
		new_center.z = center.z - size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		childs[BOTTOM_SE] = new OctreeNode(new_box);

		// SouthWest
		new_center.x = center.x - size.x * 0.25f;
		new_center.y = center.y - size.y * 0.25f;
		new_center.z = center.z - size.z * 0.25f;
		new_box.SetFromCenterAndSize(new_center, new_size);
		childs[BOTTOM_SW] = new OctreeNode(new_box);
}

void OctreeNode::RedistributeObjects()
{
	for (std::list<ComponentTransform*>::iterator it = objects.begin(); it != objects.end();)
	{
		ComponentTransform* go = *it;

		AABB new_box(go->GetAABB());

		// Now distribute this new gameobject onto the childs
		bool intersects[8];
		bool intersects_all = true;
		for (int i = 0; i < 8; ++i)
		{
			intersects[i] = childs[i]->limits.Intersects(new_box);
			if (!intersects[i])
			{
				intersects_all = false;
			 }
		}

		if (intersects_all)
			++it; // if it hits all childs, better to just keep it here
		else
		{
			it = objects.erase(it);
			for (int i = 0; i < 4; ++i)
				if (intersects[i]) childs[i]->Insert(go);
		}
	}
}


void Octree::SetLimits(const AABB & limits)
{
	Clear();
	root_node = new OctreeNode(limits);
}

void Octree::Insert(ComponentTransform * go)
{
	if(root_node!=nullptr &&
		go->GetAABB().Intersects(root_node->limits))
	{
		root_node->Insert(go);
	}
}

void Octree::Clear()
{
	RELEASE(root_node);
}
