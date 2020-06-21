#ifndef MODULESCENE_H_
#define MODULESCENE_H_

#define OCTREE_X_SIZE 500.0f
#define OCTREE_Y_SIZE 200.0f
#define OCTREE_Z_SIZE 500.0f

#include <map>
#include <string>
#include <vector>

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Octree.h"

#include "../MathGeoLib/include/Geometry/LineSegment.h"
#include "../imGuizmo/ImGuizmo.h"

#include "ComponentCamera.h"

class JSONFile;
struct ImVec4;
struct Event;
class AnimatorNode;
class ComponentAnimator;
class ModuleScene : public Module
{	
public:
	ModuleScene(const char * name, bool start_enabled = true);
	~ModuleScene();

	bool Start(JSONFile* config) override;
	update_status Update(float dt) override;
	update_status PostUpdate() override;
	void EventRequest(const Event& event) override;
	bool CleanUp();
	void GameObjectUpdateRecursive(float dt, ComponentTransform * transform);
	void GameObjectPostUpdateRecursive(ComponentTransform * object);
	bool IntersectRay(LineSegment* ray, RaycastHit& hit);
	void RecreateOctree();
	void GetStaticObjects(std::vector<ComponentTransform*>& static_objects);
	void SetMainCamera(ComponentCamera* main_camera);

	const char * GetComponentType(const uint type);
	uint GetComponentType(const char * type);

private:
	void CreateOctree();
	void DeleteGameObject(GameObject* gameobject);
	void GetIntersectBoxNonStatics(ComponentTransform * object, LineSegment* ray, std::map<float, ComponentTransform*>& out_objects);
	bool TestWithTriangles(LineSegment * ray, std::map<float, ComponentTransform*>& out_objects, RaycastHit& hit_out);
	void LoadStaticObjects();
	void LoadStreetScene();
	void DrawObjects(ComponentCamera* camera);
	void DrawWithFrustrum(ComponentCamera* camera);
	void SaveScene();
	void LoadScene(const char* scene_path);
	LineSegment ray;
	JSONFile current_scene;
	std::string current_scene_name;
public:
	Octree octree;
	
	//All gameobjects are children of the root gameobject
	GameObject* root_gameobject = nullptr;
	ComponentCamera* game_camera = nullptr;
	GameObject * character = nullptr;
	ComponentAnimator* character_animator = nullptr;
	AnimatorNode* idle = nullptr;
	AnimatorNode* attack = nullptr;
	AnimatorNode* run = nullptr;
	friend class ModuleRender3D;
};

#endif // !MODULESCENE_H_

