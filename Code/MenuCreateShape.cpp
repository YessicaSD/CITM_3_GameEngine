#include "MenuCreateShape.h"
#include "par\par_shapes.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleImport.h"
#include "Globals.h"

MenuCreateShape::MenuCreateShape() :
	panel_cube("Create cube", this),
	panel_parametric_sphere("Create parametric sphere", this),
	panel_subdivided_sphere("Create subdivided sphere", this),
	panel_hemisphere("Create hemisphere", this),
	panel_plane("Create plane", this),
	panel_klein_bottle("Create klein bottle", this),
	panel_cylinder("Create cylinder", this),
	panel_cone("Create cone", this),
	panel_torus("Create torus", this),
	panel_trefoil_knot("Create trefoil knot", this),
	panel_dodecahedron("Create dodecahedron", this),
	panel_icosahedron("Create icosahedron", this),
	panel_disk("Create disk", this)
{
	button_size.x = button_size.y = button_height;

	//Disk
	disk_center = new float[3];
	disk_center[0] = 0.f;
	disk_center[1] = 0.f;
	disk_center[2] = 0.f;

	disk_normal = new float[3];
	disk_normal[0] = 0.f;
	disk_normal[1] = 1.f;
	disk_normal[2] = 0.f;
}
MenuCreateShape::~MenuCreateShape()
{
	RELEASE_ARRAY(disk_center);
	RELEASE_ARRAY(disk_normal);
};

void MenuCreateShape::MenuBarTab()
{
	//TODO: Put in constructor
	button_color = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_Text);

	if (ImGui::BeginMenu("Create"))
	{
		MenuItem(
			"Cube",
			par_shapes_create_cube,
			panel_cube);

		MenuItem(
			"Parametric sphere",
			[slices = parametric_sphere_slices, stacks = parametric_sphere_stacks]()
			{ return par_shapes_create_parametric_sphere(slices, stacks); },
			panel_parametric_sphere);

		MenuItem(
			"Subdivided sphere",
			[nsubdivisions = subdivided_sphere_nsubdivisions]()
			{ return par_shapes_create_subdivided_sphere(nsubdivisions); },
			panel_subdivided_sphere);

		MenuItem(
			"Hemisphere",
			[slices = hemisphere_slices, stacks = hemisphere_stacks]()
			{ return par_shapes_create_hemisphere(slices, stacks); },
			panel_hemisphere);

		MenuItem(
			"Plane",
			[slices = plane_slices, stacks = plane_stacks]
			{ return par_shapes_create_plane(slices, stacks); },
			panel_plane);

		MenuItem(
			"Klein bottle",
			[slices = klein_bottle_slices, stacks = klein_bottle_stacks]
			{ return par_shapes_create_klein_bottle(slices, stacks); },
			panel_klein_bottle);

		MenuItem(
			"Cylinder",
			[slices = cylinder_slices, stacks = cylinder_stacks]()
			{ return par_shapes_create_cylinder(slices, stacks); },
			panel_cylinder);

		MenuItem(
			"Cone",
			[slices = cone_slices, stacks = cone_stacks]()
			{ return par_shapes_create_cone(slices, stacks); },
			panel_cone);

		MenuItem(
			"Torus",
			[slices = torus_slices, stacks = torus_stacks, radius = torus_radius]()
			{ return par_shapes_create_torus(slices, stacks, radius); },
			panel_torus);

		MenuItem(
			"Trefoil knot",
			[slices = trefoil_knot_slices, stacks = trefoil_knot_stacks, radius = trefoil_knot_radius]()
			{return par_shapes_create_trefoil_knot(slices, stacks, radius); },
			panel_trefoil_knot);

		MenuItem(
			"Dodecahedron",
			par_shapes_create_dodecahedron,
			panel_dodecahedron);

		MenuItem(
			"Icosahedron",
			par_shapes_create_icosahedron,
			panel_icosahedron);

		MenuItem(
			"Disk",
			[radius = disk_radius, slices = disk_slices, center = disk_center, normal = disk_normal]()
			{ return par_shapes_create_disk(radius, slices, center, normal); },
			panel_disk);


		//par_shapes_create_tetrahedron();
		//par_shapes_create_lsystem();
		//par_shapes_create_rock();

		//TODO: Show menu to configurate this
		ImGui::EndMenu();
	}
}

void MenuCreateShape::MenuItem(std::string name, std::function<par_shapes_mesh*()> mesh_function, Panel & panel)
{
	ImGui::MenuItem(name.c_str());
	ImGui::SetItemAllowOverlap();
	bool selectable_clicked = ImGui::IsItemClicked();

	ImGui::SameLine(button_space);

	//INFO: Center button vertically
	ImGuiStyle style = ImGui::GetStyle();
	ImVec2 cursor_pos = ImGui::GetCursorPos();
	cursor_pos.y += (ImGui::GetFontSize() - button_height) * 0.5f;
	ImGui::SetCursorPos(cursor_pos);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 250.f);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, button_color);
	ImGui::Button("", button_size);
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	//INFO: We're using ImGui::IsItemClicked() instead of the return of ImGui::Button because they execute on different frames
	bool button_clicked = ImGui::IsItemClicked();

	if (selectable_clicked && !button_clicked)
	{
		par_shapes_mesh* mesh = mesh_function();
		App->import->LoadParShape(name, mesh);
		par_shapes_free_mesh(mesh);
	}
	if (button_clicked)
	{
		panel.SetActive(true);
		//TODO: Open panel
		//Keep a pointer to the panel you've accessed
		//Draw it every frame the panel is closed
		//This should be done in another function, because this one is going to be executed while creating the main menu bar
	}
}

//TODO: Pass arguments
//Mainly the function to be called
void MenuCreateShape::PanelCreateMultiple()
{
	ImGui::Separator();
	ImGui::Text("Create Multiple");
}

//Panels code

PanelCreateShape::PanelCreateShape(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	Panel(name, active, shortcut),
	menu_create_shape(menu_create_shape)
{}

PanelCreateCube::PanelCreateCube(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateParametricSphere::PanelCreateParametricSphere(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateSubdividedSphere::PanelCreateSubdividedSphere(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateHemisphere::PanelCreateHemisphere(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreatePlane::PanelCreatePlane(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateKleinBottle::PanelCreateKleinBottle(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateCylinder::PanelCreateCylinder(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateCone::PanelCreateCone(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateTorus::PanelCreateTorus(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateTrefoilKnot::PanelCreateTrefoilKnot(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateDodecahedron::PanelCreateDodecahedron(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateIcosahedron::PanelCreateIcosahedron(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

PanelCreateDisk::PanelCreateDisk(std::string name, MenuCreateShape * menu_create_shape, bool active, std::vector<SDL_Scancode> shortcut) :
	PanelCreateShape(name, menu_create_shape, active, shortcut)
{}

void PanelCreateCone::Draw()
{
	ImGui::Begin("Create Cone");
	App->gui->create_menu.PanelCreateMultiple();
	if (ImGui::Button("Cancel"))
	{
		SetActive(false);
	}
	ImGui::SameLine();
	if (ImGui::Button("Create"))
	{
		par_shapes_create_cone(App->gui->create_menu.cone_slices, App->gui->create_menu.cone_stacks);
	}
	ImGui::End();
}