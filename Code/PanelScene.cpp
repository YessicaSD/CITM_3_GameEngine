#include "PanelScene.h"
#include "imgui/imgui.h"
#include "Application.h"

#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleGui.h"
#include "ModuleCamera3D.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"

#include <algorithm>
#include "Event.h"

PanelScene::PanelScene(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	Panel(name, active, shortcuts)
{ }

//PanelScene is called by ModuleGui
//ModuleScene executes draws (fills the framebuffer texture) before ModuleGui
//By doing it in this order we can get the image of this frame
void PanelScene::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin("Scene",NULL, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::Button("World"))
		{
			guizmo_mode = ImGuizmo::WORLD;
		}
		if (ImGui::Button("Local"))
		{
			guizmo_mode = ImGuizmo::LOCAL;
		}
		ImGui::EndMenuBar();
	}


	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	
	
	 width = ImGui::GetWindowWidth();
	 height = ImGui::GetWindowHeight();
	
	//GetSizeWithAspectRatio(App->window->GetWindowWidth(), App->window->GetWindowHeight(), panel_width-50, panel_height-50, width, height);
	
	ImVec2 min = ImGui::GetCursorScreenPos();
	//min = { min.x + 25, min.y + 25 };
	current_viewport_size = ImGui::GetContentRegionAvail();
	ImVec2 max = ImVec2(min.x + current_viewport_size.x, min.y + current_viewport_size.y);
	mouse_is_hovering = ImGui::IsMouseHoveringRect(min, max);
	cursor = { ImGui::GetMousePos().x - min.x,ImGui::GetMousePos().y - min.y };

	ImGui::GetWindowDrawList()->AddImage(
		(void*)App->renderer3D->scene_fbo.render_texture,
		min,
		ImVec2(min.x + width,  min.y + height),
		ImVec2(0, 1),
		ImVec2(1, 0));
	ComponentTransform* go = App->gui->GetSelectedTransform();
	if ( go != nullptr)
	{
		DrawGizmo(App->camera->GetCurrentCamera(), go);
	}

	ImGui::End();
	ImGui::PopStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding);
}

void PanelScene::DrawGizmo(ComponentCamera* camera, ComponentTransform* go)
{
	float4x4 view = camera->GetViewMatrix();
	float4x4 proj = camera->GetProjectionMatrix();

	//ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
	float4x4 model = go->GetGlobalMatrix().Transposed();

	ImGuizmo::SetRect(float(ImGui::GetCursorScreenPos().x), float(ImGui::GetCursorScreenPos().y), float(width), float(height));
	ImGuizmo::SetDrawlist();

	if (guizmo_op == ImGuizmo::OPERATION::SCALE && guizmo_mode != ImGuizmo::MODE::LOCAL)
		guizmo_mode = ImGuizmo::MODE::LOCAL;

	ImGuizmo::Manipulate((const float*)& view, (const float*)& proj.Transposed(), guizmo_op, guizmo_mode, (float*)& model);
	if (ImGuizmo::IsOver())
	{
		is_over_gizmo = true;
	}
	if (ImGuizmo::IsUsing())
	{
		is_being_used = true;
		if (go->is_static == true)
		{
			update_octree_when_stop_moving = true;
		}
		float4x4 parent = go->GetParent()->GetGlobalMatrix();
		go->SetGlobalMatrix(parent.Inverted() * model.Transposed());
	}
	else
	{
		is_being_used = false;
		if (update_octree_when_stop_moving)
		{
			
			update_octree_when_stop_moving = false;
			App->AddEvent(Event(Event::UPDATE_OCTREE));
		}
	}



}

void PanelScene::GetSizeWithAspectRatio(int current_width, int current_height, int wanted_width, int wanted_height, int & new_width, int & new_height)
{
	float scale_height = (float)wanted_height / (float)current_height;
	float scale_width = (float)wanted_width / (float)current_width;

	float scale = (std::min)(scale_height, scale_width);

	new_width = current_width * scale;
	new_height = current_height * scale;
}
