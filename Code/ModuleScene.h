#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#include <map>
#include <string>
#include <vector>

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "imGuizmo/ImGuizmo.h"
struct ImVec4;
class ComponentCamera;

class ModuleScene : public Module
{	
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start(JSONFile* config) override;
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	bool CleanUp();
	void GameObjectPostUpdateRecursive(ComponentTransform * object);
	bool IntersectRay(LineSegment* ray, RaycastHit& hit);

private:
	void DeleteGameObject(GameObject* gameobject);
	void GetIntersectBox(ComponentTransform * object, LineSegment* ray, std::vector<RaycastHit>& out_objects);
	bool TestWithTriangles(LineSegment * ray, std::vector<RaycastHit>& out_objects, RaycastHit& hit_out);
	LineSegment ray;

	


public:
	//All gameobjects are children of the root gameobject
	GameObject* root_gameobject;
	GameObject* camera;
	ComponentCamera* component_camera;
	friend class ModuleRender3D;
};

#endif // !MODULESCENE_H_

