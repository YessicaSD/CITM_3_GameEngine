

#include "Application.h"
#include "ModuleGui.h"

#include <stdio.h>
#include <string>


#include "mmgr\mmgr.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

ModuleGUI::ModuleGUI(bool start_enabled):Module(start_enabled)
{

}

bool ModuleGUI::Init()
{
	bool ret = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init(App->window->glsl_version);

	memset(fpsHistory, 0, sizeof(float) * CURRENT_FPS_MAX_VALUE);
	memset(msHistory, 0, sizeof(float) * CURRENT_FPS_MAX_VALUE);
	memset(RamHistory, 0, sizeof(float) * CURRENT_FPS_MAX_VALUE);


	char str[100];
	
	
	return ret;
}

update_status ModuleGui::PreUpdate()
{
	for (std::vector<Panels*>::iterator iter = panels.begin();
		iter != panels.end();
		++iter)
	{
		if ((*iter)->Shortcut->Pressed())
		{
			//Open the panel
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
	update_status ret = update_status::UPDATE_CONTINUE;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	static bool displayConfi = false;


	DisplayMainMenuBar(ret, displayConfi);
	if (showMenuImGui)
		ImGui::ShowDemoWindow();

	if (displayConfi) {
		DisplayConfiguration(ret, displayConfi);
	}
	

	return ret;
}

update_status ModuleGUI::PostUpdate()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return update_status::UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();


	ImGui::DestroyContext();

	return true;
}

void ModuleGUI::DisplayMainMenuBar(update_status &ret, bool &display_confi)
{
	ImGui::BeginMainMenuBar();
	//if (ImGui::BeginMenu("Help"))
	//{
	//	ImGui::Button("Documentation");
	//	ImGui::EndMenu();
	//}


	//ImGui::BeginMenu("File");
	//	if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
	//	if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
	//	if (ImGui::MenuItem("Close", "Ctrl+W")) {   }
	//	ImGui::EndMenu();
	ImGui::EndMainMenuBar();
	ImGui::Begin("Config");
	if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
	{
		SDL_SetWindowFullscreen(App->window->window, App->window->fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	}
	if (ImGui::Checkbox("Resizable", &App->window->resizable))
	{
		SDL_SetWindowResizable(App->window->window, App->window->resizable ? SDL_TRUE : SDL_FALSE);
	}
	if (ImGui::Checkbox("Borderless", &App->window->borderless))
	{
		SDL_SetWindowBordered(App->window->window, App->window->borderless ? SDL_FALSE : SDL_TRUE);
	}
	if (ImGui::Checkbox("Fullscreen Desktop", &App->window->fullscreen_desktop))
	{
		SDL_SetWindowFullscreen(App->window->window, App->window->fullscreen_desktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
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
	ImGui::End();


	// MAIN MENU BAR ======================================================================================
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Project"))
	{
		if (ImGui::MenuItem("Configuration		"))
			display_confi = true;

		ImGui::EndMenu();
	}

	

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Menu ImGui example		"))
			showMenuImGui = !showMenuImGui;
		if (ImGui::MenuItem("Documentation		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/wiki");
		if(ImGui::MenuItem("Download latest		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/releases");
		if(ImGui::MenuItem("Report a bug		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/issues");

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

}

void ModuleGUI::DisplayConfiguration(update_status & ret, bool& window_bool)
{

	if (window_bool)
	{
		ImGui::SetNextWindowSize(ImVec2(550, 680));
		ImGui::Begin("Configuration", &window_bool);

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
		ImVec2 size = {310,100};
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
		

		//Style
		if (ImGui::CollapsingHeader("Style"))
		{
			ImGuiStyle& style = ImGui::GetStyle();
			static ImGuiStyle ref_saved_style;
			if (ImGui::ShowStyleSelector("Colors##Selector"))
				ref_saved_style = style;
		}
			
		ImGui::End();

	}
	//ImGui::ShowDemoWindow();
}
