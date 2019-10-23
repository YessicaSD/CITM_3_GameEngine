#include "PanelConfiguration.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGui.h"


#include "mmgr/mmgr.h"
#include "imgui\imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>


PanelConfiguration::PanelConfiguration(std::string name, bool active, std::vector<SDL_Scancode> shortcut) :
	Panel(name, active, shortcut)
{
	memset(fpsHistory, 0, sizeof(float) * CURRENT_FPS_MAX_VALUE);
	memset(msHistory, 0, sizeof(float) * CURRENT_FPS_MAX_VALUE);
	memset(RamHistory, 0, sizeof(float) * CURRENT_FPS_MAX_VALUE);

	
}

void PanelConfiguration::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(550, 680));
	ImGui::Begin(name.c_str());
		//Project name
		static char projectName[128] = "Project name";
		ImGui::InputText("Project Name:", projectName, IM_ARRAYSIZE(projectName));
		static char projectOrganization[128] = "Early Birds";
		ImGui::InputText("Organization:", projectOrganization, IM_ARRAYSIZE(projectName));

		//FPS
		static float fpsMax = 0;
		if (ImGui::SliderFloat("Max FPS", &fpsMax, 0.0f, 60.0f, "%.0f"))
		{

		}
		ImGui::Text("Limit fps:");
		ImVec4 textColor_fpsmas = { 1.f,1.0f,0.3f,1.0f };
		ImGui::SameLine();
		//ImGui::TextColored(textColor_fpsmas, std::to_string(fpsMax).c_str());


		//FPS GRAPH ==================================
		ImVec2 size = { 310,100 };
		static int currFpsArrayIndex = 0;
		static float currFPS = 0.0f;
		char titleGraph[100];

		if (updateGraph.ReadSec() > 0.5f)
		{
			fpsHistory[currFpsArrayIndex] = currFPS = App->GetAvgFPS();

			++currFpsArrayIndex;

			if (currFpsArrayIndex >= CURRENT_FPS_MAX_VALUE)
				currFpsArrayIndex = 0;

			updateGraph.Start();
		}

		sprintf_s(titleGraph, 100, "Framerate: %.2f", currFPS);
		ImGui::PlotHistogram("##ASDFASF", fpsHistory, IM_ARRAYSIZE(fpsHistory), currFpsArrayIndex, titleGraph, 0.0f, 100.0f, size);

		//MS GRAPH ================================================
		static int lastMsArrayIndex = 0;
		static Uint32 lastFrameMs = 0.0f;

		msHistory[lastMsArrayIndex] = lastFrameMs = App->GetLastFrameMs();
		lastMsArrayIndex = (lastMsArrayIndex == CURRENT_FPS_MAX_VALUE) ? 0 : ++lastMsArrayIndex;

		sprintf_s(titleGraph, 100, "Milliseconds: %i", lastFrameMs);
		ImGui::PlotHistogram("##ASDFASF", msHistory, IM_ARRAYSIZE(msHistory), lastMsArrayIndex, titleGraph, 0.0f, 15.0f, size);

		////MEMORY CONSUMPTION =====================================
		sMStats stats = m_getMemoryStatistics();
		static int MemoryArrayIndex = 0;
		static float lastMemoryConsume = 0.0f;
		static int loops_counter = 0;

		if (++loops_counter > 10)
		{

			loops_counter = 0;
			RamHistory[MemoryArrayIndex] = lastMemoryConsume = (float)stats.totalReportedMemory;
			MemoryArrayIndex = (MemoryArrayIndex >= CURRENT_FPS_MAX_VALUE) ? 0 : ++MemoryArrayIndex;
		}

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
				ref_saved_style = style;
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