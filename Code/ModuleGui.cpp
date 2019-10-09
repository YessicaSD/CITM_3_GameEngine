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

bool ModuleGui::Init()
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

	TabPanels[(uint)TYPE_TAB_PANEL::RIGHT_TAB_PANEL].name = "Inspector";

	SetTabPanelsResized(App->window->GetWindowWidth(), App->window->GetWindowHeight());

	panel_config    = new PanelConfiguration("Configuration",true);
	panels.pop_back();
	panel_console = new PanelConsole("Console", true);
	panel_shortcuts = new PanelShortcuts("Shortcuts", true/*, { SDL_SCANCODE_Q }*/);
	TabPanels[(uint)TYPE_TAB_PANEL::RIGHT_TAB_PANEL].panels.push_back(new PanelProperties("Properties", true));
	TabPanels[(uint)TYPE_TAB_PANEL::RIGHT_TAB_PANEL].panels.push_back(panel_config);

	return ret;
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

	DisplayMainMenuBar(ret);
	if (showMenuImGui)
		ImGui::ShowDemoWindow();

	return ret;
}

update_status ModuleGui::PostUpdate()
{
	// Rendering
	for (uint i = 0; i < (uint)TYPE_TAB_PANEL::MAX_TAB_PANEL; ++i)
	{
		TabPanel& tab = TabPanels[i];
		//ImGui::SetNextWindowPos(ImVec2((float)tab.x, (float)tab.y), ImGuiCond_Always);
		//ImGui::SetNextWindowSize(ImVec2((float)tab.width, (float)tab.height), ImGuiCond_Always);
		//static bool tabBarOpen;
		if (ImGui::Begin(tab.name/*nullptr*//*, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing*/))
		{
			
				if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
				{
					for (std::vector<Panel*>::iterator iter = tab.panels.begin(); iter != tab.panels.end(); ++iter)
					{
						if (ImGui::BeginTabItem((*iter)->GetName()))
						{

							if ((*iter)->IsActive())
							{
								(*iter)->Draw();
							}

							ImGui::EndTabItem();
						}

					}
					ImGui::EndTabBar();
				}
			
			
		
			

		}
		ImGui::End();
	}

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
	for (std::vector<Panel*>::reverse_iterator iter = panels.rbegin(); iter != panels.rend(); ++iter)
	{
		if ((*iter))
		{
			delete (*iter);
			(*iter) = nullptr;

		}
	}
	panels.clear();
	panel_console = nullptr;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();


	ImGui::DestroyContext();

	return true;
}

void ModuleGui::Log(const char *sentence)
{
	if(panel_console)
		panel_console->Log(sentence);
}

void ModuleGui::DisplayMainMenuBar(update_status &ret)
{
	
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


	
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Project"))
	{
		ImGui::EndMenu();
	}
	
	if (ImGui::BeginMenu("Render Mode"))
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

		ImGui::EndMenu();
	}

	

	ImGui::EndMainMenuBar();


	

}

void ModuleGui::AddInputLog(SDL_Scancode key, KEY_STATE state)
{
	panel_config->AddInputLog(key, state);
}

void ModuleGui::ModifyShortcut(SDL_Scancode key)
{
	panel_shortcuts->ModifyShortcut(key);
}

void ModuleGui::SetTabPanelsResized(int width, int height)
{
	TabPanels[(uint)TYPE_TAB_PANEL::RIGHT_TAB_PANEL].width = 400;
	TabPanels[(uint)TYPE_TAB_PANEL::RIGHT_TAB_PANEL].height = height;
	TabPanels[(uint)TYPE_TAB_PANEL::RIGHT_TAB_PANEL].y = 21;
	TabPanels[(uint)TYPE_TAB_PANEL::RIGHT_TAB_PANEL].x = width - TabPanels[(uint)TYPE_TAB_PANEL::RIGHT_TAB_PANEL].width;

}



