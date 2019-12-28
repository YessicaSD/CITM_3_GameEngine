#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleGui.h"

#include <stdio.h>
#include <string>


#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imGuizmo/ImGuizmo.h"

#include "Timer.h"
#include "Panel.h"
#include "PanelConfiguration.h"
#include "PanelShortcuts.h"
#include "PanelConsole.h"
#include "PanelProperties.h"
#include "PanelAbout.h"
#include "PanelHierarchy.h"
#include "PanelAssets.h"
#include "PanelScene.h"
#include "PanelResources.h"
#include "PanelGame.h"
#include "PanelImport.h"
#include "PanelTools.h"

//TODO: Frame Buffer Object remove
#include "par/par_shapes.h"
#include "ModuleImportModel.h"
#include "Event.h"

#define IMGUI_LIGHT_GREY ImVec4(0.8f,0.8f,0.8f,1.f)
#define IMGUI_GREY ImVec4(0.6f,0.6f,0.6f,1.f)
#define IMGUI_BLUE ImVec4(0.2f,0.2f,1.f,1.f)
#define IMGUI_GREEN ImVec4(0.f,1.f,0.f,1.f)
#define IMGUI_YELLOW ImVec4(1.f,1.f,0.f,1.f)
#define IMGUI_RED ImVec4(1.f,0.f,0.f,1.f)
#define IMGUI_WHITE ImVec4(1.f,1.f,1.f,1.f)

#define NORMAL_TEXT_COLOR IMGUI_LIGHT_GREY
#define TITLE_1_TEXT_COLOR IMGUI_GREY

#include "NodeEditor/Include/imgui_node_editor.h"

namespace ed = ax::NodeEditor;

static ed::EditorContext* g_Context = nullptr;

ModuleGui::ModuleGui(const char * name, bool start_enabled):Module(start_enabled, name)
{

}

ModuleGui::~ModuleGui()
{
	RELEASE(create_menu);
}

bool ModuleGui::Init(JSONFile * module_file)
{
	bool ret = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	LOG("Creating ImGui Context");
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
	ImGui_ImplOpenGL3_Init(App->window->glsl_version);

	ed::Config config;
	config.SettingsFile = "Simple.json";
	g_Context = ed::CreateEditor(&config);

	return ret;
}

bool ModuleGui::Start(JSONFile * module_file)
{
	panel_console		= CreatePanel<PanelConsole>("Console", true);
	panel_shortcuts		= CreatePanel<PanelShortcuts>("Shortcuts", true);
	panel_hierarchy		= CreatePanel<PanelHierarchy>("Hirearchy", true);
	panel_properties	= CreatePanel<PanelProperties>("Properties", true);
	panel_configuration	= CreatePanel<PanelConfiguration>("Configuration", true);
	panel_assets		= CreatePanel<PanelAssets>("Assets", true);
	panel_about			= CreatePanel<PanelAbout>("About", true);
	panel_scene			= CreatePanel<PanelScene>("Scene", true);
	panel_resources		= CreatePanel<PanelResources>("Resources", true);
	panel_game			= CreatePanel<PanelGame>("Game", true);
	panel_import		= CreatePanel<PanelImport>("Import", true);
	panel_tools			= CreatePanel<PanelTools>("Tools", true);
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
	

	ImGuiIO& io = ImGui::GetIO();

	//Dockspace needs to be created before any other window
	//They won't be able to attack to it otherwise
	CreateDockspace(io);

	MainMenuBar(ret);

	ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

	ImGui::Separator();

	ed::SetCurrentEditor(g_Context);
	ed::Begin("My Editor", ImVec2(0.0, 0.0f));
	int uniqueId = 1;
	// Start drawing nodes.
	ed::BeginNode(uniqueId++);
	ImGui::Text("Node A");
	ed::BeginPin(uniqueId++, ed::PinKind::Input);
	ImGui::Text("-> In");
	ed::EndPin();
	ImGui::SameLine();
	ed::BeginPin(uniqueId++, ed::PinKind::Output);
	ImGui::Text("Out ->");
	ed::EndPin();
	ed::EndNode();
	ed::End();
	ed::SetCurrentEditor(nullptr);

	for (std::vector<Panel*>::iterator iter = panels.begin(); iter != panels.end(); ++iter)
	{
		if ((*iter)->IsActive())
		{
			(*iter)->Draw();
		}
	}
	if (showMenuImGui)
	{
		ImGui::ShowDemoWindow(&showMenuImGui);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//Viewports
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	return ret;
}

void ModuleGui::CreateDockspace(ImGuiIO& io)
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	{
		window_flags |= ImGuiWindowFlags_NoBackground;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &dockspace_active, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
		LOG("ERROR: Docking is not enabled!");
	}

	ImGui::End();
}

bool ModuleGui::CleanUp()
{
	ed::DestroyEditor(g_Context);

	if (create_menu)
	{
		delete create_menu;
		create_menu = nullptr;
	}
	
	for (std::vector<Panel*>::reverse_iterator iter = panels.rbegin(); iter != panels.rend(); ++iter)
	{
		if ((*iter))
		{
			
			LOG("%s", (*iter)->GetName());
			delete (*iter);

		}
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

void ModuleGui::EventRequest(const Event & event)
{
	if (event.type == Event::CHANGE_HIERARCHY)
	{
		panel_hierarchy->ChangeHierarchy();
	}
}

void ModuleGui::SetSelectedGameObjec(ComponentTransform * gameobject)
{
	if (selected_transform != nullptr)
	{
		selected_transform->SetSelected(false);
	}

	selected_transform = gameobject;
	panel_properties->selected_transform = gameobject;

	if (gameobject != nullptr)
	{
		gameobject->SetSelected(true);
		OpenInHierarchy(gameobject);
	}

	

}

void ModuleGui::MainMenuBar(update_status &ret)
{	
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save Scene"))
		{
			App->AddEvent(Event(Event::SAVE_SCENE));
		}
		if (ImGui::MenuItem("Quit"))
		{
			ret = UPDATE_STOP;
		}
		ImGui::EndMenu();
	}

	//create_menu->MenuBarTab();

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
		if (ImGui::MenuItem("Resources", NULL, &panel_resources->active))
		{
			panel_resources->SetActive(panel_resources->active);
		}
		if (ImGui::MenuItem("Game", NULL, &panel_game->active))
		{
			panel_game->SetActive(panel_game->active);
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

void ModuleGui::OpenInHierarchy(ComponentTransform * gameobject)
{
	ComponentTransform* parent = gameobject->GetParent();
	if (parent != nullptr)
	{
		parent->open_in_hierarchy = true;
		OpenInHierarchy(parent);
	}
}


void ModuleGui::ModifyShortcut(SDL_Scancode key)
{
	panel_shortcuts->ModifyShortcut(key);
}

