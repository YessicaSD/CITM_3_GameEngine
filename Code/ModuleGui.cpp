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
	panel_console		= CreatePanel<PanelConsole>("Console", true);
	panel_shortcuts		= CreatePanel<PanelShortcuts>("Shortcuts", true);
	panel_hierarchy = CreatePanel<PanelHierarchy>("Hirearchy", true);
	panel_properties	= CreatePanel<PanelProperties>("Properties", true);
	panel_configuration	= CreatePanel<PanelConfiguration>("Configuration", true);
	panel_assets		= CreatePanel<PanelAssets>("Assets", true);
	panel_about			= CreatePanel<PanelAbout>("About", true);

	create_menu = new MenuCreateShape();

	return true;
}

update_status ModuleGui::PreUpdate()
{
	for (std::vector<Panel*>::iterator iter = panels.begin();
		iter != panels.end();
		++iter)
	{
		if ((*iter)->shortcut->Pressed())
		{
			(*iter)->SwitchActive();
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate()
{
	update_status ret = UPDATE_CONTINUE;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	MainMenuBar(ret);

	for (std::vector<Panel*>::iterator iter = panels.begin(); iter != panels.end(); ++iter)
	{
		if ((*iter)->IsActive())
		{
			(*iter)->Draw();
		}
	}
	if (showMenuImGui)
	{
		ImGui::ShowDemoWindow();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return ret;
}

bool ModuleGui::CleanUp()
{
	if (create_menu)
	{
		delete create_menu;
		create_menu = nullptr;
	}
	
	for (std::vector<Panel*>::reverse_iterator iter = panels.rbegin(); iter != panels.rend(); ++iter)
	{
		if ((*iter))
			delete (*iter);
	}
	panels.clear();
	panel_console = nullptr;
	
	if (int size = shortcuts.size() > 0)
	{
		for (std::vector<Shortcut*>::iterator iter = shortcuts.begin(); iter != shortcuts.end(); ++iter)
		{
			if((*iter))
				delete ((*iter));
		}
		shortcuts.clear();
	}
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

		if (ImGui::MenuItem("Hierarchy", NULL, &panel_hierarchy->active))
		{
			panel_console->SetActive(panel_hierarchy->active);
		}
		if(ImGui::MenuItem("Properties", NULL, &panel_properties->active))
		{
			panel_properties->SetActive(panel_properties->active);
		}
		if (ImGui::MenuItem("Assets", NULL, &panel_assets->active))
		{
			panel_assets->SetActive(panel_assets->active);
		}
		if (ImGui::MenuItem("Debug console", NULL, &panel_console->active))
		{
			panel_console->SetActive(panel_console->active);
		}
		if (ImGui::MenuItem("Configuration", NULL, &panel_configuration->active))
		{
			panel_configuration->SetActive(panel_configuration->active);
		}
		if (ImGui::MenuItem("Shortcuts", NULL, &panel_shortcuts->active))
		{
			panel_shortcuts->SetActive(panel_shortcuts->active);
		}
		ImGui::EndMenu();
	}
	
	if (ImGui::BeginMenu("Render Mode All"))
	{
		if (ImGui::Checkbox("Fill", &render_mode_all.fill))
		{
			//TODO: Activate / deactivate render mode for every game object
		}
		if (ImGui::Checkbox("Wireframe", &render_mode_all.wireframe))
		{
		}
		if (ImGui::Checkbox("Points", &render_mode_all.point))
		{
		}
		if (ImGui::Checkbox("Vertices Normals", &render_mode_all.vertex_normals))
		{
		}
		if (ImGui::Checkbox("Face Normals", &render_mode_all.face_normals))
		{
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Menu ImGui example		"))
		{
			showMenuImGui = !showMenuImGui;
		}
		if (ImGui::MenuItem("Documentation		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/wiki");
		if(ImGui::MenuItem("Download latest		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/releases");
		if(ImGui::MenuItem("Report a bug		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/issues");
		if (ImGui::MenuItem("License		"))
			App->RequestBrowser("https://github.com/YessicaSD/CITM_3_GameEngine/blob/master/LICENSE.md");
		
		if(ImGui::MenuItem("About", NULL, &panel_about->active))
		{
			panel_about->SetActive(panel_about->active);
		}

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}


void ModuleGui::ModifyShortcut(SDL_Scancode key)
{
	panel_shortcuts->ModifyShortcut(key);
}

