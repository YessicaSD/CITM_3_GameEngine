#ifndef COMPONENT_ANIMATOR_H
#define COMPONENT_ANIMATOR_H

#include "Component.h"
#include <map>
#include <string>
class ResourceAnimation;
class ComponentTransform;
class AnimatorNode
{
private:
	ResourceAnimation* clip = nullptr;
	float speed = 1.f;
public:
	void SetClip(ResourceAnimation* clip);
	ResourceAnimation* GetClip();
	float current_time = 0.f;
	bool loop = true;
};

class EntryState
{
	AnimatorNode* defaulState = nullptr;
};

class Animator
{
	
};

class ComponentAnimator : public Component
{
	CLASS_DECLARATION(ComponentAnimator);

private:
	void DrawBoneRecursive(ComponentTransform * bone) const;
public:
	ComponentAnimator(GameObject* attached_object);
	void AddClip(ResourceAnimation* clip);
	//TODO: RemoveClip
	void PropertiesEditor() override;
	void OnUpdate(float dt) override;
	void OnPostUpdate() override;
	ComponentTransform * GetBoneByName(const std::string & bone_name);
	void CleanUp() override;

private:
	std::vector<AnimatorNode*> nodes;
	std::map<std::string, ComponentTransform*> bones;

	friend class PanelTimeline;
};


#endif // !COMPONENT_ANIMATION_H

