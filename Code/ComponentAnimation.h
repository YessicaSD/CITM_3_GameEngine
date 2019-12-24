#ifndef COMPONENT_ANIMATION_H
#define COMPONENT_ANIMATION_H
#include "Component.h"
#include <map>
#include <string>
class ResourceAnimation;
class ComponentTransform;
class AnimatorNode
{
	ResourceAnimation* clip = nullptr;
	float speed = 1;
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

public:
	ComponentAnimator(GameObject* attached_object);
	void AddClip(ResourceAnimation* clip);
	//TODO: RemoveClip
	void PropertiesEditor() override;
	void OnPostUpdate() override;
	ComponentTransform * GetBoneByName(const std::string & bone_name);
	void CleanUp() override;
private:
	std::vector<ResourceAnimation*> clips;
	std::map<std::string, ComponentTransform*> bones;
};


#endif // !COMPONENT_ANIMATION_H

