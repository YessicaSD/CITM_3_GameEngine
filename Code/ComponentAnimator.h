#ifndef COMPONENT_ANIMATOR_H
#define COMPONENT_ANIMATOR_H

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include <map>
#include <string>
class ResourceAnimation;
class ComponentTransform;
class AnimatorNode
{
private:
	ResourceAnimation *clip = nullptr;

	

public:
	float speed = 1;
	std::string name;
	AnimatorNode(std::string name) : name(name){};
	void SetClip(ResourceAnimation *clip);
	ResourceAnimation *GetClip();
	float current_time = 0.f;
	bool loop = true;
	~AnimatorNode();
};

class EntryState
{
	AnimatorNode *defaulState = nullptr;
};

class Animator
{
};
struct trs
{
	float3 pos, scale;
	Quat rot;
public:
	trs() {};
	trs(float3 pos, float3 scale, Quat rot) :pos(pos), scale(scale), rot(rot)
	{

	}
};
class ComponentAnimator : public Component
{
	CLASS_DECLARATION(ComponentAnimator);

private:
	void DrawBoneRecursive(ComponentTransform *bone) const;
	void SaveBonesState(std::map<std::string, trs>& map, AnimatorNode* node, double current_time_ticks);
	void MoveBones(ResourceAnimation * resource_animation, double current_time_ticks);
	void DoTransition();

	void IterateBonesTransition(ComponentTransform * iter, float time);

public:
	ComponentAnimator(GameObject *attached_object);
	AnimatorNode* AddClip(ResourceAnimation *clip);
	//TODO: RemoveClip
	void PropertiesEditor() override;
	void OnUpdate(float dt) override;
	
	void OnPostUpdate() override;
	ComponentTransform *GetBoneByName(const std::string &bone_name);
	void CleanUp() override;
	std::vector<ComponentTransform*> root_nodes;
private:
	std::map<std::string, trs> current_bones;
	std::map<std::string, trs> next_bones;
	AnimatorNode* current_animation_node = nullptr;
	AnimatorNode* next_node = nullptr;
	uint curr_node_idx = 0;
	
	std::vector<AnimatorNode *> nodes;
	std::map<std::string, ComponentTransform *> bones;
	float transition_time = 1; //seconds
	bool start_transition = false;
	float time_of_transition = 0;
	friend class PanelTimeline;
};

#endif // !COMPONENT_ANIMATION_H
