#include "ComponentAnimator.h"
#include "ResourceAnimation.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTime.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "imgui/imgui_timeline.h"

#include "glew\include\GL\glew.h"

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
		AnimatorNode * animator_node = new AnimatorNode();
		animator_node->SetClip(clip);
		nodes.push_back(animator_node);
	}
}

void ComponentAnimator::PropertiesEditor()
{
	ImGui::Separator();
	//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	if (CollapsigHeaderWithCheckbox())
	{
		ImGui::Text("Open the timeline window to see the animation's timline.");
	}
	//TODO: Show a list of all the clips and let you select which one has to be played
}

void ComponentAnimator::OnUpdate(float dt)
{
	if (nodes.size() > 0u)
	{
		ResourceAnimation* resource_animation = nodes[0]->GetClip();
		if (resource_animation != nullptr)
		{
			nodes[0]->current_time += App->GetDt();
			if (nodes[0]->current_time >= resource_animation->GetDuration())
			{
				nodes[0]->current_time = (nodes[0]->loop) ? 0 : resource_animation->GetDuration();
			}
			float current_time_ticks = nodes[0]->current_time * resource_animation->GetTicksPerSecond();
			uint num_channels = resource_animation->GetNumChannels();
			AnimationChannels* channels = resource_animation->GetChannels();

			for (uint i = 0; i < num_channels; ++i)
			{
				AnimationChannels channel = channels[i];
				ComponentTransform * bone = GetBoneByName(channel.GetName());

				if (bone != nullptr)
				{
					float3 position_key;
					if (!channel.GetKeyPosition(current_time_ticks, position_key))
					{
						position_key = bone->GetPosition();
					}
					float3 scale_key;
					if (!channel.GetKeyScale(current_time_ticks, scale_key))
					{
						scale_key = bone->GetScale();
					}
					Quat rotation_key;
					if (!channel.GetKeyRotation(current_time_ticks, rotation_key))
					{
						rotation_key = bone->GetRotation();
					}
					bone->SetTransform(position_key, scale_key, rotation_key);
				}
			}
		}
	}
}

void ComponentAnimator::OnPostUpdate()
{
	//Draw the bones
	
	//Go throgh all the childs recursively and draw a line between the parent and the children
	
	DrawBoneRecursive(gameobject->transform);

	//Draw
	//glEnableClientState(GL_VERTEX_ARRAY);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);//vertex
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indice);//indices
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentAnimator::DrawBoneRecursive(ComponentTransform * bone) const
{
	glColor3f(0.2f, 1.f, 0.25f);
	glLineWidth(5.f);

	float3 bone_pos = bone->GetGlobalPosition();
	auto children = bone->GetChildren();
	for (auto child : children)
	{
		float3 child_pos = child->GetGlobalPosition();
		glBegin(GL_LINES);
		glVertex3f(bone_pos.x, bone_pos.y, bone_pos.z);
		glVertex3f(child_pos.x, child_pos.y, child_pos.z);
		glEnd();

		DrawBoneRecursive(child);
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
	for (uint i = 0; i < nodes.size(); ++i)
	{
		nodes[i]->GetClip()->StopUsingResource();
		RELEASE(nodes[i]);
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