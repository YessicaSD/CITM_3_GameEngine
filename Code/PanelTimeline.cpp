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
		if (animator->nodes.size() > 0u)
		{
			AnimatorNode * animator_node = animator->nodes[0];
			//TODO: Let you select the node you want to see

			ImGui::BeginTimeline("animation timeline", animator_node->GetClip()->GetDuration());
			float values[] = { 0.1, 0.2 };
			ImGui::TimelineEvent("event 01", values);
			ImGui::TimelineEvent("event 02", values);
			ImGui::TimelineMarker(animator_node->current_time);
			ImGui::EndTimeline();
		}
		else
		{
			ImGui::Text("This animator doesn't have any nodes.");
		}
	}
	else
	{
		ImGui::Text("Select a gameobject with an Animator component in the hierarchy to display its animation timeline.");
	}
	ImGui::End();
}
