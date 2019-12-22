#include "ComponentAnimation.h"
#include "ResourceAnimation.h"
#include "imgui/imgui.h"

CLASS_DEFINITION(Component, ComponentAnimation);

ComponentAnimation::ComponentAnimation(GameObject * attached_object): Component(attached_object)
{
};

void ComponentAnimation::SetAnimationClip(ResourceAnimation* clip)
{
	if (resource_animation != nullptr)
	{
		resource_animation->StopUsingResource();
	}
	if (clip != nullptr)
	{
		clip->StartUsingResource();
		resource_animation = clip;
	}
}

void ComponentAnimation::OnPostUpdate()
{
	if (resource_animation != nullptr)
	{
		
	}
}

void ComponentAnimation::PropertiesEditor()
{
	ImGui::Separator();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	if (ImGui::CollapsingHeader(name.c_str()))
	{

	}
}
