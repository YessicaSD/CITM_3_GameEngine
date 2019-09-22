//#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew/lib/Release/Win32/glew32.lib")
#pragma comment (lib, "glew/lib/Release/Win32/glew32s.lib")
#pragma comment (lib, "glfw/lib-vc2017/glfw3.lib")

#include "ModuleGui.h"
#include "Application.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>//TODO: Remove if it's not really needed

#include "glew/include/GL/glew.h"

#include "glfw/include/GLFW/glfw3.h"
#include "glfw/include/GLFW/glfw3native.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

ModuleGUI::ModuleGUI(Application* app, bool start_enabled):Module(app, start_enabled)
{

}

bool ModuleGUI::Start()
{
	bool ret = true;

	//TODO: Maybe this should be on the renderer

	//// Setup window
	//glfwSetErrorCallback(glfw_error_callback);
	//if (!glfwInit())
	//	return 1;

	//// Decide GL+GLSL versions
	//const char* glsl_version = "#version 130";
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	//// Create window with graphics context
	//GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	//if (window == NULL)
	//	return 1;
	//glfwMakeContextCurrent(window);
	//glfwSwapInterval(1); // Enable vsync

	//// Initialize OpenGL loader
	//bool err = glewInit() != GLEW_OK;
	//if (err)
	//{
	//	fprintf(stderr, "Failed to initialize OpenGL loader!\n");
	//	ret = false;
	//}

	//// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;

	//ImGui::StyleColorsDark();

	//// Setup Platform/Renderer bindings
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui_ImplOpenGL3_Init(glsl_version);

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
