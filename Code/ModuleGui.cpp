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
		static char projectName[128] = "Project name";
		ImGui::InputText("Project Name:", projectName, IM_ARRAYSIZE(projectName));
		ImGui::End();

	}
	ImGui::ShowDemoWindow();

	//static bool show_demo_window = false;
	//static bool show_another_window = false;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//ImGui::ShowDemoWindow(&show_demo_window);

	//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;

	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);

	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::End();
	//}

	
}
