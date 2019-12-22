#ifndef COMPONENT_ANIMATION_H
#define COMPONENT_ANIMATION_H
#include "Component.h"

class AnimationState
{

};

class EntryState
{
	AnimationState* defaulState = nullptr;
};

class Animator
{
	
};
class ResourceAnimation;
class ComponentAnimation : public Component
{
	CLASS_DECLARATION(ComponentAnimation);

	ResourceAnimation* resource_animation = nullptr;
	
public:
	ComponentAnimation(GameObject* attached_object);
	void SetAnimationClip(ResourceAnimation* clip);
	void OnPostUpdate() override;
	void PropertiesEditor() override;

};


#endif // !COMPONENT_ANIMATION_H

