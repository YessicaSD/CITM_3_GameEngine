#include "ComponentAnimator.h"
#include "ResourceAnimation.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTime.h"

#include "GameObject.h"
#include "ComponentTransform.h"

#include "imgui/imgui_timeline.h"
#include "ModuleResourceManager.h"

#include "glew\include\GL\glew.h"
#include "Globals.h"

CLASS_DEFINITION(Component, ComponentAnimator);

ComponentAnimator::ComponentAnimator(GameObject *attached_object) : Component(attached_object)
{
	name = "Animator";
};

void ComponentAnimator::AddClip(ResourceAnimation *clip)
{
	//TODO: Check is not a duplicate
	if (clip != nullptr)
	{
		AnimatorNode *node = new AnimatorNode("New node"); //We need to Load the resource first before getting its name (that's done inside SetClip() > StartUsingResource() > LoadData())
		node->SetClip(clip);
		node->name = clip->GetName() + std::string(" | ") + clip->asset_source;
		//TODO remove this later
		if (nodes.size() == 0)
		{
			
				curr_node_idx = 0;
			current_animation_node = node;
		}
		nodes.push_back(node);
	}
}

void ComponentAnimator::PropertiesEditor()
{
	ImGui::Separator();
	//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	if (CollapsigHeaderWithCheckbox())
	{
		ImGui::Text("Animator nodes:");
		for (auto node = nodes.begin(); node != nodes.end(); ++node)
		{
			ImGui::Text((*node)->name.c_str());
			std::string str_speed = "## speed";
			str_speed += (*node)->name;
			ImGui::InputFloat(str_speed.c_str(), &(*node)->speed);
			ImGui::Text("Loop:");
			ImGui::SameLine();
			std::string str_check = "##";
			str_check += (*node)->name.c_str();
			ImGui::Checkbox(str_check.c_str(), &(*node)->loop);

			ImGui::Separator();
		}
		if (ImGui::BeginMenu("Add node with clip."))
		{
			//TODO: Don't search for clips each time
			//TODO: Don't use the string to compare the type of the clip
			std::vector<ResourceAnimation *> all_clips;
			//Collect all the clips
			for (auto iter = App->resource_manager->resources.begin(); iter != App->resource_manager->resources.end(); ++iter)
			{
				if (strcmp((*iter).second->GetTypeString(), "animation") == 0)
				{
					all_clips.push_back((ResourceAnimation *)(*iter).second);
				}
			}

			for (auto iter = all_clips.begin(); iter != all_clips.end(); ++iter)
			{
				char buffer[UID_DIGITS];
				sprintf_s(buffer, "%020llu", (*iter)->GetUID());
				if (ImGui::MenuItem((std::string(buffer) + " | " + (*iter)->asset_source).c_str()))
				{
					AddClip((*iter));
				}
			}
			ImGui::EndMenu();
		}
	}
	//TODO: Show a list of all the clips and let you select which one has to be played
}

void ComponentAnimator::OnUpdate(float dt)
{
	if (current_animation_node != nullptr)
	{
		ResourceAnimation *resource_animation = current_animation_node->GetClip();
		if (resource_animation != nullptr)
		{
			current_animation_node->current_time += App->GetDt() * current_animation_node->speed;
			double current_time_ticks = current_animation_node->current_time * resource_animation->GetTicksPerSecond();
			//----
			// Do transition to next animation
			if (!current_animation_node->loop && nodes.size() > 1u
				&& !start_transition
				&& current_animation_node->current_time >= resource_animation->GetDuration())
			{
				if ((curr_node_idx + 1u) < nodes.size())
				{
					++curr_node_idx;
				}
				else
				{
					curr_node_idx = 0u;
				}
				next_node = nodes[curr_node_idx];
				SaveBonesState(current_bones, current_animation_node, current_time_ticks);
				SaveBonesState(next_bones, next_node, 0.);
				start_transition = true;
				return;
			}
			// Do loop or mantain at the end, the last one only happens if there is only one animation
			else if (current_animation_node->current_time >= resource_animation->GetDuration())
			{
				if (current_animation_node->loop)
				{
					current_animation_node->current_time = 0.f;
				}
				else
				{
					current_animation_node->current_time = resource_animation->GetDuration();
					//TODO: Don't update buffers, they stay the same
				}
			}

			if (start_transition)
			{
				time_of_transition += App->GetDt();
				DoTransition();
				if (time_of_transition > 1.f)
				{
					start_transition = false;
					time_of_transition = 0.f;
				}
			}
			else
			{
				MoveBones(resource_animation, current_time_ticks);
			}
		}
	}
}

void ComponentAnimator::MoveBones(ResourceAnimation *resource_animation, double current_time_ticks)
{
	uint num_channels = resource_animation->GetNumChannels();
	AnimationChannels *channels = resource_animation->GetChannels();
	for (uint i = 0; i < num_channels; ++i)
	{
		AnimationChannels channel = channels[i];
		ComponentTransform *bone = GetBoneByName(channel.GetName());

		if (bone != nullptr)
		{
			float3 position_key;
			if (!channel.GetKeyPosition(current_time_ticks, position_key))
			{
				position_key = bone->GetPosition();
			}
			float3 scale_key = {1, 1, 1};
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

void ComponentAnimator::DoTransition()
{
	for (auto iter = root_nodes.begin(); iter != root_nodes.end(); ++iter)
	{
		IterateBonesTransition((*iter), time_of_transition);
	}
}

void ComponentAnimator::IterateBonesTransition(ComponentTransform* iter, float time)
{
	const char* bone_name = iter->gameobject->GetName();
	auto start_key = current_bones.find(bone_name);
	auto end_key = next_bones.find(bone_name);
	trs start_trans(iter->GetPosition(),iter->GetScale(), iter->GetRotation());
	trs end_trans = start_trans;
	trs result;
	if (start_key != current_bones.end())
	{
		start_trans = (*start_key).second;
	}
	if (end_key != next_bones.end())
	{
		end_trans = (*end_key).second;
	}
	result.pos = start_trans.pos.Lerp(end_trans.pos, time);
	result.scale = start_trans.scale.Lerp(end_trans.scale, time);
	result.rot = start_trans.rot.Slerp(end_trans.rot, time);
	iter->SetTransform(result.pos, result.scale, result.rot);

	std::vector<ComponentTransform*> childs = (*iter).GetChildren();
	for (auto child = childs.begin(); child != childs.end(); ++child)
	{
		IterateBonesTransition((*child), time);
	}
}

void ComponentAnimator::OnPostUpdate()
{
	//Draw the bones

	//Go throgh all the childs recursively and draw a line between the parent and the children

	for (auto root_bone = root_nodes.begin(); root_bone != root_nodes.end(); ++root_bone)
	{
		DrawBoneRecursive((*root_bone));
	}

	//Draw
	//glEnableClientState(GL_VERTEX_ARRAY);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);//vertex
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indice);//indices
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentAnimator::DrawBoneRecursive(ComponentTransform *bone) const
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

void ComponentAnimator::SaveBonesState(std::map<std::string, trs> &map, AnimatorNode *node, double current_time_ticks)
{
	ResourceAnimation *clip = current_animation_node->GetClip();
	AnimationChannels *channels = clip->GetChannels();
	map.clear();
	for (uint i = 0; i < clip->GetNumChannels(); ++i)
	{
		AnimationChannels channel = channels[i];
		float3 position_key;
		channel.GetKeyPosition(current_time_ticks, position_key);
		float3 scale_key = {1, 1, 1};
		channel.GetKeyScale(current_time_ticks, scale_key);
		Quat rotation_key;
		channel.GetKeyRotation(current_time_ticks, rotation_key);
		map[channel.GetName()] = trs(position_key, scale_key, rotation_key);
	}
}

//INFO: Optimization, adds it to a map so we don't have to iterate over all the children gameobjects all the time
ComponentTransform *ComponentAnimator::GetBoneByName(const std::string &bone_name)
{
	ComponentTransform *bone = nullptr;

	std::map<std::string, ComponentTransform *>::iterator bone_iter = bones.find(bone_name);
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
}


void AnimatorNode::SetClip(ResourceAnimation *clip)
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

ResourceAnimation *AnimatorNode::GetClip()
{
	return clip;
}

AnimatorNode::~AnimatorNode()
{
	if (clip)
	{
		clip->StopUsingResource();
		clip = nullptr;
	}
}
