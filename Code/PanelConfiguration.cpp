#include "PanelConfiguration.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGui.h"

#include "mmgr\mmgr.h"

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

	//Render options
	depth_test = glIsEnabled(GL_DEPTH_BUFFER) == GL_TRUE;
	cull_face  = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
	lighting = glIsEnabled(GL_LIGHTING) == GL_TRUE;
	color_material = glIsEnabled(GL_COLOR_MATERIAL) == GL_TRUE;
	texture_2d = glIsEnabled(GL_TEXTURE_2D) == GL_TRUE;
}

void PanelConfiguration::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(550, 680));
	if (ImGui::CollapsingHeader("Configuration"))
	{
		//Project name
		static char projectName[128] = "Project name";
		ImGui::InputText("Project Name:", projectName, IM_ARRAYSIZE(projectName));

		//FPS
		static float fpsMax = 0;
		if (ImGui::SliderFloat("Max FPS", &fpsMax, 0.0f, 60.0f, "%.0f"))
		{

		}
		ImGui::Text("Limit fps:");
		ImVec4 textColor_fpsmas = { 1.f,1.0f,0.3f,1.0f };
		ImGui::SameLine();
		ImGui::TextColored(textColor_fpsmas, std::to_string(fpsMax).c_str());


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

		sprintf_s(titleGraph, 100, "Ram Consume: %.2f", lastMemoryConsume);
		ImGui::PlotHistogram("##ASDFASF", RamHistory, IM_ARRAYSIZE(RamHistory), MemoryArrayIndex, titleGraph, 0.0f, (float)stats.peakReportedMemory * 2.f, size);
		ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
		ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
		ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
		ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);


		//STYLE ===========================================================
		if (ImGui::CollapsingHeader("Style"))
		{
			ImGuiStyle& style = ImGui::GetStyle();
			static ImGuiStyle ref_saved_style;
			if (ImGui::ShowStyleSelector("Colors##Selector"))
				ref_saved_style = style;
		}

		
	}

	if (ImGui::CollapsingHeader("Render"))
	{
		ImGui::Text("Change background color:");

		static float default_color_background[3] = { 3 / 255.F,19 / 255.F,29 / 255.F };
		static float col1[3] = { 1.0f,0.0f,0.2f };
		if (ImGui::ColorPicker4("MyColor##4", col1))
		{
			glClearColor(col1[0], col1[1] , col1[2] , 1);
		}
		if(ImGui::Button("Reset"))
		{
			glClearColor(default_color_background[0], default_color_background[1], default_color_background[2], 1);

		}

		ImGui::Text("Window options");
		const char * window_modes[] = {
			"No fullscreen",
			"Fullscreen",
			"Fullscreen desktop" };
		if (ImGui::Combo("Window mode", &current_window_mode, window_modes, IM_ARRAYSIZE(window_modes)))
		{
			Uint32 change_mode = 0;
			if (current_window_mode == 0)
			{
				change_mode = 0;
			}
			else if (current_window_mode == 1)
			{
				change_mode = SDL_WINDOW_FULLSCREEN;
			}
			else if (current_window_mode == 2)
			{
				change_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
			}
			SDL_SetWindowFullscreen(App->window->window, change_mode);
		}

		if (ImGui::Checkbox("Resizable", &App->window->resizable))
		{
			SDL_SetWindowResizable(App->window->window, App->window->resizable ? SDL_TRUE : SDL_FALSE);
		}
		if (ImGui::Checkbox("Borderless", &App->window->borderless))
		{
			SDL_SetWindowBordered(App->window->window, App->window->borderless ? SDL_FALSE : SDL_TRUE);
		}
		if (ImGui::Checkbox("Vsync", &App->window->vsync))
		{
			if (SDL_GL_SetSwapInterval(App->window->vsync ? 1 : 0) == -1)
				//1 for updates synchronized with the vertical retrace
				//0 for immediate updates
			{
				LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			}
		}

		ImGui::Text("Render options");

		RenderOption("Depth test", &depth_test, GL_DEPTH_TEST);
		RenderOption("Cull faces", &cull_face, GL_CULL_FACE);
		RenderOption("Lighting", &lighting, GL_LIGHTING);
		RenderOption("Color materials", &color_material, GL_COLOR_MATERIAL);
		RenderOption("Texture 2D", &texture_2d, GL_TEXTURE_2D);

	}

	if (ImGui::CollapsingHeader("Input"))
	{
		int mouse_x = App->input->GetMouseX();
		int mouse_y = App->input->GetMouseY();
		ImGui::Text("Mouse Position:");
		ImGui::SameLine();
		ImGui::Text("%i,%i", mouse_x, mouse_y);

		int mouse_motion_x = App->input->GetMouseMotionX();
		int mouse_motion_y = App->input->GetMouseMotionY();
		ImGui::Text("Mouse Motion:");
		ImGui::SameLine();
		ImGui::Text("%i,%i", mouse_x, mouse_y);

		int mouse_wheel = App->input->GetMouseWheel();
		ImGui::Text("Mouse Wheel:");
		ImGui::SameLine();
		ImGui::Text("%i", mouse_wheel);

		ImGui::Separator();

		ImGui::BeginChild("Input Log");
		ImGui::TextUnformatted(input_log_buffer.begin());
		if (scroll_input_log)
		{
			ImGui::SetScrollHere(1.0f);
		}
		scroll_input_log = false;
		
		ImGui::EndChild();
	}


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
}

void PanelConfiguration::AddInputLog(SDL_Scancode key, KEY_STATE state)
{
	std::string scancode_name = SDL_GetScancodeName(key);
	std::string new_entry = "Key: " + scancode_name + ". \n";
	input_log_buffer.appendf(new_entry.c_str());
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