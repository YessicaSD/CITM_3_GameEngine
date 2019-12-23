#include "ComponentAnimation.h"
#include "ResourceAnimation.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTime.h"
#include "GameObject.h"
#include "ComponentTransform.h"
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
		uint num_channels = resource_animation->GetNumChannels();
		AnimationChannels* channels = resource_animation->GetChannels();
		for (uint i = 0; i < num_channels; ++i)
		{
			AnimationChannels channel = channels[i];
			std::string bone_name = channel.GetName();
			std::map<std::string, ComponentTransform*>::iterator bone_iter = bones.find(bone_name);
			ComponentTransform* bone = nullptr;
			if (bone_iter != bones.end())
			{ 
				bone = (*bone_iter).second;
			}
			else
			{
				bone = gameobject->transform->Find(bone_name.c_str());
				if (bone != nullptr)
				{
					bones[bone_name] = bone;
				}
			}
			if (bone != nullptr)
			{
				KeyAnimation<float3>* position_key = channel.getKeyPosition(current_time);
				KeyAnimation<float3>* scale_key = channel.getKeyScale(current_time);
				KeyAnimation<Quat>* rotation_key = channel.getKeyRotation(current_time);
				
				float3 end_position = (position_key != nullptr) ? position_key->value : bone->GetPosition();
				float3 end_scale = (scale_key != nullptr) ? scale_key->value : bone->GetScale();
				Quat end_rotation = (rotation_key != nullptr) ? rotation_key->value : bone->GetRotation();
				bone->SetTransform(end_position, end_scale, end_rotation);
			}
		}
	}
}
