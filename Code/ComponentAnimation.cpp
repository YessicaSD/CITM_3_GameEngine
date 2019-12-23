#include "ComponentAnimation.h"
#include "ResourceAnimation.h"
#include "imgui/imgui.h"

CLASS_DEFINITION(Component, ComponentAnimation);

ComponentAnimation::ComponentAnimation(GameObject * attached_object): Component(attached_object)
{
};

void ComponentAnimation::AddClip(ResourceAnimation* clip)
{
	//TODO: Check is not a duplicate
	if (clip != nullptr)
	{
		clip->StartUsingResource();
		clips.push_back(clip);
	}
}

void ComponentAnimation::PropertiesEditor()
{
	ImGui::Separator();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	if (ImGui::CollapsingHeader(name.c_str()))
	{

	}
	//TODO: Show a list of all the clips and let you select which one has to be played
}
