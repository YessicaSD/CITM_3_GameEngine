#ifndef MODULEGUI_H_
#define MODULEGUI_H_

#include "Module.h"

#include "imgui\imgui.h"
#include <vector>
#include "MenuCreateShape.h"
#include "ComponentMesh.h"

class Timer;
class Panel;
class Shortcut;
class PanelConfiguration;
class PanelConsole;
class PanelShortcuts;
class PanelProperties;
class PanelInspector;
class PanelHierarchy;
class PanelAbout;
class PanelAssets;
class PanelScene;
class ComponentTransform;

enum KEY_STATE;

struct ImVec2;


class ModuleGui : public Module
{
public:
	MenuCreateShape * create_menu = nullptr;
	PanelScene * panel_scene = nullptr;

private:
	bool showMenuImGui = false;
	std::vector<Panel*> panels;
	PanelConfiguration* panel_configuration = nullptr;
	PanelConsole* panel_console = nullptr;
	PanelShortcuts * panel_shortcuts = nullptr;
	PanelProperties* panel_properties = nullptr;
	PanelAbout* panel_about = nullptr;
	PanelHierarchy * panel_hierarchy = nullptr;
	PanelAssets* panel_assets = nullptr;
	std::vector<Shortcut *> shortcuts;
	ComponentTransform* selected_transform = nullptr;

public:
	ModuleGui(const char * name, bool start_enabled = true);
	~ModuleGui();
	bool Init(JSON_Object* config) override;
	bool Start(JSON_Object* config) override;
	update_status PreUpdate() override;
	update_status PostUpdate() override;
	void CreateDockspace(ImGuiIO& io);
	bool CleanUp() override;
	bool Log(const char*);

	void AddInputLog(SDL_Scancode key, KEY_STATE state);
	void ModifyShortcut(SDL_Scancode key);

	template <class PanelClass>
	PanelClass * CreatePanel(std::string name, bool active = false, std::vector<SDL_Scancode> shortcuts = {})
	{
		PanelClass * new_panel = new PanelClass(name, active, shortcuts);
		panels.push_back(new_panel);
		return new_panel;
	}

	ComponentTransform* GetSelecteTransform()
	{
		return selected_transform;
	}
	void SetSelectedGameObjec(ComponentTransform* gameobject);
	

private:
	void MainMenuBar(update_status &ret);
	bool dockspace_active = true;

	RenderMode render_mode_all;

	friend class Shortcut;
	friend class Panel;
	friend class PanelShortcuts;
	friend class PanelHierarchy;
	friend class ModuleCamera3D;
	friend class PanelProperties;
};

#endif // !MODULEGUI_H_

