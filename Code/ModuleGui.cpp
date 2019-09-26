#include <string>
#include "ModuleGui.h"
#include "Application.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"



ModuleGUI::ModuleGUI(Application* app, bool start_enabled):Module(app, start_enabled)
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

	return ret;
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
	if(displayConfi)
		DisplayConfiguration(ret, displayConfi);

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
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
	int lineSpace = 8;//Extra space because it's too close to the left
	ImVec2 buttonSize;
	buttonSize.x = 50;
	buttonSize.y = 20;


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
		if (ImGui::MenuItem("Documentation		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/wiki");
		if(ImGui::MenuItem("Download latest		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/releases");
		if(ImGui::MenuItem("Report a bug		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/issues");

		ImGui::EndMenu();
	}


	//ImGui::Indent(App->window->GetWindowWidth() - buttonSize.x - lineSpace);
	//if (ImGui::Button("X", buttonSize))
	//{
	//	ret = update_status::UPDATE_STOP;
	//}

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
		ImGui::Text("Limit text:");
		ImVec4 textColor_fpsmas = { 1.f,1.0f,0.3f,1.0f };
		ImGui::SameLine();
		ImGui::TextColored(textColor_fpsmas, std::to_string(fpsMax).c_str());
		
		// Capture the group size and create widgets using the same size
		ImVec2 size = {100,50};
		const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
		ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

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
	ImGui::ShowDemoWindow();

	
	
}
