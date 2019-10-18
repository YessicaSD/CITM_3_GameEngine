#ifndef __MENU_CREATE_SHAPE_H__
#define __MENU_CREATE_SHAPE_H__

#include "imgui/imgui.h"
#include <functional>
#include "Panel.h"
#include <vector>

typedef struct par_shapes_mesh_s par_shapes_mesh;
class MenuCreateShape;

class ShapeValue
{
public:
	ShapeValue(std::string name, ImGuiDataType data_type, void * value_ptr);

public:
	std::string name;
	ImGuiDataType data_type;
	void * value_ptr;
};

class PanelCreateShape : public Panel
{
public:
	PanelCreateShape(std::string name, bool active = false, std::vector<SDL_Scancode> shortcut = {});
private:
	void Draw() override;
	void CreateMultiple();//Function to create multiple is going to be the same for each one of them

	//Function to call(may be the same that the MenuCreateShape::MenuItem() calls)
public:
	std::function<par_shapes_mesh*()> mesh_function;
	std::vector<ShapeValue> shape_values;
};

//A class to handle the create shapes menu which lets you create different geometric shapes
class MenuCreateShape
{
public:
	MenuCreateShape();
	~MenuCreateShape();

	void MenuBarTab();

private:
	void PanelCreateMultiple();
	void MenuItem(std::string name, std::function<par_shapes_mesh*()> mesh_function, Panel * panel);

	//TODO: Load variables with a function from ModuleGui

public:

private:
	const float button_height = 7.5f;
	const float button_space = 150.f;
	ImVec4 button_color;
	ImVec2 button_size;

	//Parametric sphere
	int parametric_sphere_slices = 10;
	int parametric_sphere_stacks = 10;

	//Subdivided sphere
	int subdivided_sphere_nsubdivisions = 1;

	//Hemisphere
	int hemisphere_slices = 50;
	int hemisphere_stacks = 20;

	//Plane
	int plane_slices = 10;
	int plane_stacks = 10;

	//Klein bottle
	int klein_bottle_slices = 10;
	int klein_bottle_stacks = 10;

	//Cylinder
	int cylinder_slices = 50;
	int cylinder_stacks = 10;

	//Cone variables
	int cone_slices = 12;
	int cone_stacks = 12;

	//Torus variables
	int torus_slices = 12;
	int torus_stacks = 12;
	float torus_radius = 0.5f;

	//Trefoil knot
	int trefoil_knot_slices = 50;
	int trefoil_knot_stacks = 20;
	float trefoil_knot_radius = 2.f;

	//Disk
	float * disk_center = nullptr;
	float * disk_normal = nullptr;
	int disk_slices = 12;
	float disk_radius = 12.f;

	//INFO: We create a different panel for each because we might want to customize them individually
	PanelCreateShape* panel_cube = nullptr;
	PanelCreateShape* panel_parametric_sphere = nullptr;
	PanelCreateShape* panel_subdivided_sphere = nullptr;
	PanelCreateShape* panel_hemisphere = nullptr;
	PanelCreateShape* panel_plane = nullptr;
	PanelCreateShape* panel_klein_bottle = nullptr;
	PanelCreateShape* panel_cylinder = nullptr;
	PanelCreateShape* panel_cone = nullptr;
	PanelCreateShape* panel_torus = nullptr;
	PanelCreateShape* panel_trefoil_knot = nullptr;
	PanelCreateShape* panel_dodecahedron = nullptr;
	PanelCreateShape* panel_icosahedron = nullptr;
	PanelCreateShape* panel_disk = nullptr;
};

#endif