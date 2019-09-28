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

	DisplayGui(ret);

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

void ModuleGUI::DisplayGui(update_status &ret)
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Help"))
	{
		ImGui::Button("Documentation");
		ImGui::EndMenu();
	}


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
	//if (ImGui::BeginMenu("Config")) {

	//	ImGui::EndMenu();
	//}

}
