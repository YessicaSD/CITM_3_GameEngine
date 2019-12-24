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
	current_animation_node = new AnimatorNode();
};

void ComponentAnimator::AddClip(ResourceAnimation* clip)
{
	//TODO: Check is not a duplicate
	if (clip != nullptr)
	{
		clip->StartUsingResource();
		clips.push_back(clip);

		//TODO remove this later
		if (clips.size() == 1)
		{
			current_animation_node->SetClip(clip);
		}
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
		ResourceAnimation* resource_animation = current_animation_node->GetClip();
		if (resource_animation != nullptr)
		{
			current_animation_node->current_time += App->GetDt();
			if (current_animation_node->current_time >= resource_animation->GetDuration())
			{
				current_animation_node->current_time = (current_animation_node->loop) ? 0 : resource_animation->GetDuration();
			}
			float current_time_ticks = current_animation_node->current_time * resource_animation->GetTicksPerSecond();
			uint num_channels = resource_animation->GetNumChannels();
			AnimationChannels* channels = resource_animation->GetChannels();

			for (uint i = 0; i < num_channels; ++i)
			{
				AnimationChannels channel = channels[i];
				ComponentTransform * bone = GetBoneByName(channel.GetName());

				if (bone != nullptr)
				{
					KeyAnimation<float3>* position_key = channel.GetKeyPosition(current_time_ticks);
					KeyAnimation<Quat>* rotation_key = channel.GetKeyRotation(current_time_ticks);
					KeyAnimation<float3>* scale_key = channel.GetKeyScale(current_time_ticks);

					float3 end_position = (position_key != nullptr) ? position_key->value : bone->GetPosition();
					Quat end_rotation = (rotation_key != nullptr) ? rotation_key->value : bone->GetRotation();
					float3 end_scale = (scale_key != nullptr) ? scale_key->value : bone->GetScale();

					bone->SetTransform(end_position, end_scale, end_rotation);
				}
			}
		}
		
}


//INFO: Optimization, adds it to a map so we don't have to iterate over all the children gameobjects all the time
ComponentTransform* ComponentAnimator::GetBoneByName(const std::string & bone_name)
{
	ComponentTransform* bone = nullptr;

	std::map<std::string, ComponentTransform*>::iterator bone_iter = bones.find(bone_name);
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

	return bone;
}

void ComponentAnimator::CleanUp()
{
	for (uint i = 0; i < clips.size(); ++i)
	{
		clips[i]->StopUsingResource();
	}
}

void AnimatorNode::SetClip(ResourceAnimation * clip)
{
	if (clip != nullptr)
	{
		if (this->clip != nullptr)
		{
			this->clip->StopUsingResource();
		}
		this->clip = clip;
		this->clip->StartUsingResource();
	}
}

ResourceAnimation * AnimatorNode::GetClip()
{
	return clip;
}
