#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#define OCTREE_X_SIZE 200.0f
#define OCTREE_Y_SIZE 100.0f
#define OCTREE_Z_SIZE 200.0f

#include <map>
#include <string>
#include <vector>

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Octree.h"

#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "imGuizmo/ImGuizmo.h"
struct ImVec4;
struct Event;
class ComponentCamera;

class ModuleScene : public Module
{	
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start(JSONFile* config) override;
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	void EventRequest(const Event& event) override;
	bool CleanUp();
	void GameObjectPostUpdateRecursive(ComponentTransform * object);
	bool IntersectRay(LineSegment* ray, RaycastHit& hit);
	void RecreateOctree();
	void GetStaticObjects(std::vector<ComponentTransform*>& static_objects);
	Octree octree;

private:
	void CreateOctree();
	void DeleteGameObject(GameObject* gameobject);
	void GetIntersectBoxNonStatics(ComponentTransform * object, LineSegment* ray, std::map<float, ComponentTransform*>& out_objects);
	bool TestWithTriangles(LineSegment * ray, std::map<float, ComponentTransform*>& out_objects, RaycastHit& hit_out);
	void LoadStaticObjects();
	void DrawObjects();

	void DrawWithFrustrum();

	LineSegment ray;
	

public:
	//All gameobjects are children of the root gameobject
	GameObject* root_gameobject;
	GameObject* camera;
	ComponentCamera* component_camera;
	friend class ModuleRender3D;
};

#endif // !MODULESCENE_H_

