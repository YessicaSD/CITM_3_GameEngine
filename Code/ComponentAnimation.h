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

	
public:
	ComponentAnimation(GameObject* attached_object);
	void AddClip(ResourceAnimation* clip);
	//TODO: RemoveClip
	void PropertiesEditor() override;

private:
	std::vector<ResourceAnimation*> clips;
};


#endif // !COMPONENT_ANIMATION_H

