#include "MenuCreateShape.h"
#include "par\par_shapes.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleImport.h"
#include "Globals.h"

MenuCreateShape::MenuCreateShape()
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

void MenuCreateShape::Display()
{
	//TODO: Put in constructor
	button_color = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_Text);

	if (ImGui::BeginMenu("Create"))
	{
		MenuItem(
			"Cube",
			par_shapes_create_cube);

		MenuItem(
			"Parametric sphere",
			[slices = parametric_sphere_slices, stacks = parametric_sphere_stacks]()
			{ return par_shapes_create_parametric_sphere(slices, stacks); });

		MenuItem(
			"Subdivided sphere",
			[nsubdivisions = subdivided_sphere_nsubdivisions]()
			{ return par_shapes_create_subdivided_sphere(nsubdivisions); });

		MenuItem(
			"Hemisphere",
			[slices = hemisphere_slices, stacks = hemisphere_stacks]()
			{ return par_shapes_create_hemisphere(slices, stacks); });

		MenuItem(
			"Plane",
			[slices = plane_slices, stacks = plane_stacks]
			{ return par_shapes_create_plane(slices, stacks); });

		MenuItem(
			"Klein bottle",
			[slices = klein_bottle_slices, stacks = klein_bottle_stacks]
			{ return par_shapes_create_klein_bottle(slices, stacks); });

		MenuItem(
			"Cylinder",
			[slices = cylinder_slices, stacks = cylinder_stacks]()
			{ return par_shapes_create_cylinder(slices, stacks); });

		MenuItem(
			"Cone",
			[slices = cone_slices, stacks = cone_stacks]()
			{ return par_shapes_create_cone(slices, stacks); });

		MenuItem(
			"Torus",
			[slices = torus_slices, stacks = torus_stacks, radius = torus_radius]()
			{ return par_shapes_create_torus(slices, stacks, radius); });

		MenuItem(
			"Trefoil knot",
			[slices = trefoil_knot_slices, stacks = trefoil_knot_stacks, radius = trefoil_knot_radius]()
			{return par_shapes_create_trefoil_knot(slices, stacks, radius); });

		MenuItem(
			"Dodecahedron",
			par_shapes_create_dodecahedron);

		MenuItem(
			"Icosahedron",
			par_shapes_create_icosahedron);

		MenuItem(
			"Disk",
			[radius = disk_radius, slices = disk_slices, center = disk_center, normal = disk_normal]()
			{ return par_shapes_create_disk(radius, slices, center, normal); });


		//par_shapes_create_tetrahedron();
		//par_shapes_create_lsystem();
		//par_shapes_create_rock();

		//TODO: Show menu to configurate this
		ImGui::EndMenu();
	}
}

void MenuCreateShape::MenuItem(std::string name, std::function<par_shapes_mesh*()> mesh_function)
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
		//TODO: Open panel
	}
}