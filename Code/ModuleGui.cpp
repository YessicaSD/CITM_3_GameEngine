#include "ModuleGui.h"
#include "Application.h"

#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled):Module(app, start_enabled)
{


}

bool ModuleGUI::Start()
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleGUI::Update(float dt)
{
	update_status ret = update_status::UPDATE_CONTINUE;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	DisplayGui(ret);

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
}

bool ModuleGUI::CleanUp()
{
	bool ret = true;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//TODO: Put them on the renderer
	//glfwDestroyWindow(window);
	//glfwTerminate();
	return ret;
}

void ModuleGUI::DisplayGui(update_status &ret)
{
	int lineSpace = 8;//Extra space because it's too close to the left
	ImVec2 buttonSize;
	buttonSize.x = 50;
	buttonSize.y = 20;

	ImGui::BeginMainMenuBar();
	//ImGui::Spacing(100);
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//LOG("io x: %i", io.DisplaySize.x);
	//LOG("window x: %i", App->window->GetWindowWidth());
	ImGui::Indent(App->window->GetWindowWidth() - buttonSize.x - lineSpace);
	if (ImGui::Button("X", buttonSize))
	{
		ret = update_status::UPDATE_STOP;
	}
	ImGui::EndMainMenuBar();

	//ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
	//ImVec2 windowPos;
	//windowPos.x = 0;
	//windowPos.y = 500;
	//ImGui::OpenPopup("my pop up");
	//bool active = true;
	//ImGui::BeginPopupModal("my pop up modal", &active, );
	//ImGui::SetCursorScreenPos(windowPos);
	//ImGui::Begin("My window");
	//ImGui::Text("This is my window");
	//ImGui::End();
}
