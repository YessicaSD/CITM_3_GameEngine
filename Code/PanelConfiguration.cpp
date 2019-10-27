#include "PanelConfiguration.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleGui.h"


//#include "mmgr/mmgr.h"
#include "imgui\imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>


PanelConfiguration::PanelConfiguration(std::string name, bool active, std::vector<SDL_Scancode> shortcut) :
	Panel(name, active, shortcut)
{
}

void PanelConfiguration::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(550, 680));
	ImGui::Begin(name.c_str());

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save"))
		{
			App->SaveModulesConfiguration();
		}
		if(ImGui::MenuItem("Load"))
		{
			App->LoadModulesConfiguration();
		}
		ImGui::EndMenu();
	}

	App->DrawAppConfigUI();

	////MEMORY CONSUMPTION =====================================
	//sMStats stats = m_getMemoryStatistics();
	//static int MemoryArrayIndex = 0;
	//static float lastMemoryConsume = 0.0f;
	//static int loops_counter = 0;

	//if (++loops_counter > 10)
	//{

	//	loops_counter = 0;
	//	RamHistory[MemoryArrayIndex] = lastMemoryConsume = (float)stats.totalReportedMemory;
	//	MemoryArrayIndex = (MemoryArrayIndex >= CURRENT_FPS_MAX_VALUE) ? 0 : ++MemoryArrayIndex;
	//}

	//sprintf_s(titleGraph, 100, "Ram Consume: %.2f", lastMemoryConsume);
	//ImGui::PlotHistogram("##ASDFASF", RamHistory, IM_ARRAYSIZE(RamHistory), MemoryArrayIndex, titleGraph, 0.0f, (float)stats.peakReportedMemory * 2.f, size);
	//ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
	//ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
	//ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
	//ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
	//ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
	//ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
	//ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
	//ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
	//ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);


	//STYLE ===========================================================
	if (ImGui::CollapsingHeader("Style"))
	{
		ImGuiStyle& style = ImGui::GetStyle();
		static ImGuiStyle ref_saved_style;
		if (ImGui::ShowStyleSelector("Colors##Selector"))
		{
			ref_saved_style = style;
		}
	}

	App->DrawModulesConfigUi();
	
	//if (ImGui::CollapsingHeader("Render Mode"))
	//{
	//	static int style_idx = -1;
	//	if (ImGui::Combo("Mode", &style_idx, "Default\0Wireframe\0Vertex"))
	//	{
	//		switch (style_idx)
	//		{
	//		case 0:
	//		{

	//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//		}
	//		break;
	//		case 1:
	//		{

	//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//		}
	//		break;

	//		case 2:
	//			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//			break;

	//		}
	//	}
	//}
	ImGui::End();
}



void PanelConfiguration::RenderOption(const char * checkbox_string, bool *value, int gl_option)
{
	if (ImGui::Checkbox(checkbox_string, value))
	{
		if (*value)
		{
			glEnable(gl_option);
		}
		else
		{
			glDisable(gl_option);
		}
	}
}