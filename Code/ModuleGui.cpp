#include "Application.h"
#include "ModuleGui.h"

#include <stdio.h>
#include <string>


#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Timer.h"
#include "Panel.h"
#include "PanelConfiguration.h"
#include "PanelShortcuts.h"
#include "PanelConsole.h"
#include "PanelProperties.h"
#include "PanelAbout.h"
#include "PanelHierarchy.h"
#include "PanelInspector.h"
#include "PanelAssets.h"

#define IMGUI_LIGHT_GREY ImVec4(0.8f,0.8f,0.8f,1.f)
#define IMGUI_GREY ImVec4(0.6f,0.6f,0.6f,1.f)
#define IMGUI_BLUE ImVec4(0.2f,0.2f,1.f,1.f)
#define IMGUI_GREEN ImVec4(0.f,1.f,0.f,1.f)
#define IMGUI_YELLOW ImVec4(1.f,1.f,0.f,1.f)
#define IMGUI_RED ImVec4(1.f,0.f,0.f,1.f)
#define IMGUI_WHITE ImVec4(1.f,1.f,1.f,1.f)

#define NORMAL_TEXT_COLOR IMGUI_LIGHT_GREY
#define TITLE_1_TEXT_COLOR IMGUI_GREY

ModuleGui::ModuleGui(bool start_enabled):Module(start_enabled)
{

}

ModuleGui::~ModuleGui()
{
	RELEASE(create_menu);
}

bool ModuleGui::Init()
{
	bool ret = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init(App->window->glsl_version);


	return ret;
}

bool ModuleGui::Start()
{
	panel_console = CreatePanel<PanelConsole>("Console", true);
	panel_shortcuts = CreatePanel<PanelShortcuts>("Shortcuts", true);
	panel_hirearchy = CreatePanel<PanelHierarchy>("Hirearchy", true);
	CreatePanel<PanelProperties>("Properties", true);
	panel_config = CreatePanel<PanelConfiguration>("Configuration", true);
	
	CreatePanel<PanelAbout>("About", true);
	panel_assets = CreatePanel<PanelAssets>("Assets", true);
	panel_console = CreatePanel <PanelConsole>("Console", true);

	create_menu = new MenuCreateShape();

	return true;
}

update_status ModuleGui::PreUpdate()
{
	for (std::vector<Panel*>::iterator iter = panels.begin();
		iter != panels.end();
		++iter)
	{
		if ((*iter)->shortcut.Pressed())
		{
			(*iter)->SwitchActive();
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update(float dt)
{
	update_status ret = update_status::UPDATE_CONTINUE;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	MainMenuBar(ret);
	if (showMenuImGui)
		ImGui::ShowDemoWindow();

	return ret;
}

update_status ModuleGui::PostUpdate()
{
	for (std::vector<Panel*>::iterator iter = panels.begin(); iter != panels.end(); ++iter)
	{
		if ((*iter)->IsActive())
		{
			(*iter)->Draw();
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return update_status::UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp()
{
	RELEASE(create_menu);
	for (std::vector<Panel*>::reverse_iterator iter = panels.rbegin(); iter != panels.rend(); ++iter)
	{
		RELEASE((*iter));
	}
	panels.clear();
	panel_console = nullptr;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();


	ImGui::DestroyContext();

	return true;
}

bool ModuleGui::Log(const char *sentence)
{
	if (panel_console)
	{
		panel_console->Log(sentence);
		return true;
	}
	return false;
}

void ModuleGui::MainMenuBar(update_status &ret)
{	
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit"))
		{
			ret = UPDATE_STOP;
		}
		ImGui::EndMenu();
	}

	create_menu->MenuBarTab();

	if (ImGui::BeginMenu("Windows"))
	{
		bool console_state = panel_console->IsActive();
		if (ImGui::MenuItem("Console window			", NULL, &console_state))
		{
			panel_console->SwitchActive();

		}

		ImGui::EndMenu();
	}
	
	if (ImGui::BeginMenu("RenderMode"))
	{
		static bool wireframe_view = App->scene->GetRenderMode("wireframe");
		static bool default_view = App->scene->GetRenderMode("default");
		static bool vertices_view = App->scene->GetRenderMode("vertex");
		static bool vertices_normals_view = App->scene->GetRenderMode("vertices_normals");
		static bool face_normals_view = App->scene->GetRenderMode("face_normals");

		if (ImGui::MenuItem("Wireframe			", NULL, &wireframe_view))
		{
			App->ChangeRenderMode("wireframe");
		}
		if (ImGui::MenuItem("Default			", NULL, &default_view))
		{
			App->ChangeRenderMode("default");

		}
		if (ImGui::MenuItem("Verteces			", NULL, &vertices_view))
		{
			App->ChangeRenderMode("vertex");
		}
		if (ImGui::MenuItem("Vertices Normals			", NULL, &vertices_normals_view))
		{
			App->ChangeRenderMode("vertices_normals");
		}
		
		if (ImGui::MenuItem("Face Normals			", NULL, &face_normals_view))
		{
			App->ChangeRenderMode("face_normals");
		}
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
		if (ImGui::MenuItem("License		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/blob/master/LICENSE.md");
	

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}


void ModuleGui::ModifyShortcut(SDL_Scancode key)
{
	panel_shortcuts->ModifyShortcut(key);
}

