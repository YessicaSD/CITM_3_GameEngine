#include "PanelTimeline.h"
#include "Application.h"
#include "ModuleGui.h"
#include "PanelProperties.h"
#include "GameObject.h"
#include "ComponentAnimator.h"
#include "imgui/imgui_timeline.h"
#include "ResourceAnimation.h"

PanelTimeline::PanelTimeline(std::string name, bool state, std::vector<SDL_Scancode> shortcuts) : Panel(name, state, shortcuts)
{
}

void PanelTimeline::Draw()
{
	ImGui::Begin(name.c_str());
	ComponentAnimator * animator = nullptr;
	const ComponentTransform * selected_object = App->gui->panel_properties->GetSelecteTransform();
	if (selected_object != nullptr)
	{
		animator = selected_object->gameobject->GetComponent<ComponentAnimator>();
	}

	if (animator != nullptr)
	{
		//if (animator->nodes.size() > 0u)
		//{
		//	AnimatorNode * animator_node = animator->nodes[animator->curr_node_idx];
		//	ResourceAnimation * animation = animator_node->GetClip();
		//	//TODO: Let you select the node you want to see

		//	ImGui::BeginTimeline("animation timeline", animation->GetDuration());
		//	for (int i = 0; i < 3/*animation->num_channels*/; ++i)
		//	{
		//		DisplayTransformationChannel3Coord(
		//			(std::string(animation->channels[i].name) + std::string(" position")).c_str(),
		//			animation->channels[i].num_position_keys,
		//			animation->channels[i].position_keys);
		//		DisplayTransformationChannel4Coord(
		//			(std::string(animation->channels[i].name) + std::string(" rotation")).c_str(),
		//			animation->channels[i].num_rotation_keys,
		//			animation->channels[i].rotation_keys);
		//		DisplayTransformationChannel3Coord(
		//			(std::string(animation->channels[i].name) + std::string(" scale")).c_str(),
		//			animation->channels[i].num_scale_keys,
		//			animation->channels[i].scale_keys);
		//	}

		//	ImGui::TimelineMarker(animator_node->current_time);
		//	ImGui::EndTimeline();
		//}
		//else
		//{
		//	ImGui::Text("This animator doesn't have any nodes.");
		//}
	}
	else
	{
		ImGui::Text("Select a gameobject with an Animator component in the hierarchy to display its animation timeline.");
	}
	ImGui::End();
}

void PanelTimeline::DisplayTransformationChannel3Coord(const char * channel_name, int num_values, KeyAnimation<math::float3> * keys)
{
	std::string * tooltips = new std::string[num_values];
	float * times = new float[num_values];
	for (int j = 0; j < num_values; ++j)
	{
		tooltips[j] =
			std::string("x : ") + std::to_string(keys[j].value.x) + "\n" +
			std::string("y : ") + std::to_string(keys[j].value.y) + "\n" +
			std::string("z : ") + std::to_string(keys[j].value.z);
		times[j] = keys[j].time;
	}
	ImGui::TimelineChannel(channel_name, tooltips, times, num_values);
	delete[]tooltips;
	delete[]times;
}

void PanelTimeline::DisplayTransformationChannel4Coord(const char * channel_name, int num_values, KeyAnimation<math::Quat> * keys)
{
	std::string * tooltips = new std::string[num_values];
	float * times = new float[num_values];
	for (int j = 0; j < num_values; ++j)
	{
		tooltips[j] =
			std::string("x : ") + std::to_string(keys[j].value.x) + "\n" +
			std::string("y : ") + std::to_string(keys[j].value.y) + "\n" +
			std::string("z : ") + std::to_string(keys[j].value.y) + "\n" +
			std::string("w : ") + std::to_string(keys[j].value.w);
		times[j] = keys[j].time;
	}
	ImGui::TimelineChannel(channel_name, tooltips, times, num_values);
	delete[]tooltips;
	delete[]times;
}
