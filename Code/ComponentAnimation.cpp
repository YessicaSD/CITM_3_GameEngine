#include "ComponentAnimation.h"
#include "ResourceAnimation.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTime.h"

CLASS_DEFINITION(Component, ComponentAnimator);

ComponentAnimator::ComponentAnimator(GameObject * attached_object): Component(attached_object)
{
	name = "Animator";
};

void ComponentAnimator::AddClip(ResourceAnimation* clip)
{
	//TODO: Check is not a duplicate
	if (clip != nullptr)
	{
		clip->StartUsingResource();
		clips.push_back(clip);
	}
}

void ComponentAnimator::PropertiesEditor()
{
	ImGui::Separator();
	//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	if (ImGui::CollapsingHeader(name.c_str()))
	{

	}
	//TODO: Show a list of all the clips and let you select which one has to be played
}

void ComponentAnimator::OnPostUpdate()
{
	if (clips.size() > 0)
	{
		ResourceAnimation* resource_animation = (*clips.begin());
		float current_time = App->time->GetTime();
		uint num_channels;
	}
}
