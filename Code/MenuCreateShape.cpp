#include "MenuCreateShape.h"
#include "par\par_shapes.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleImport.h"

MenuCreateShape::MenuCreateShape()
{
	button_size.x = button_size.y = button_height;
}

void MenuCreateShape::Display()
{
	//TODO: Put in constructor
	button_color = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_Text);

	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Cube"))
		{
			par_shapes_mesh* mesh = par_shapes_create_cube();
			App->import->LoadParShape("Cube", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Parametric sphere"))
		{
			par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(10, 10);
			App->import->LoadParShape("Parametric sphere", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Subdivided sphere"))
		{
			par_shapes_mesh* mesh = par_shapes_create_subdivided_sphere(1);
			App->import->LoadParShape("Subdivided sphere", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Hemisphere"))
		{
			par_shapes_mesh* mesh = par_shapes_create_hemisphere(50, 20);
			App->import->LoadParShape("Hemisphere", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Plane"))
		{
			par_shapes_mesh* mesh = par_shapes_create_plane(10, 10);
			App->import->LoadParShape("Plane", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Klein"))
		{
			par_shapes_mesh* mesh = par_shapes_create_klein_bottle(10, 10);
			App->import->LoadParShape("Klein", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Cylinder"))
		{
			par_shapes_mesh* mesh = par_shapes_create_cylinder(50, 10);
			App->import->LoadParShape("Cylinder", mesh);
			par_shapes_free_mesh(mesh);
		}

		std::function<par_shapes_mesh*()> cone_function = [slices = cone_slices, stacks = cone_stacks] () { return par_shapes_create_cone(slices, stacks); };
		MenuItem("Cone", cone_function);

		//TODO: Options for creating shapes
		if (ImGui::MenuItem("Torus"))
		{
			par_shapes_mesh* mesh = par_shapes_create_torus(12, 12, 0.5f);
			App->import->LoadParShape("Torus", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Trefoil"))
		{
			par_shapes_mesh* mesh = par_shapes_create_trefoil_knot(50, 20, 2);
			App->import->LoadParShape("Trefoil", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Dodecahedron"))
		{
			par_shapes_mesh* mesh = par_shapes_create_dodecahedron();
			App->import->LoadParShape("Dodecahedron", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Icosahedron"))
		{
			par_shapes_mesh* mesh = par_shapes_create_icosahedron();
			App->import->LoadParShape("Icosahedron", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Disk"))
		{
			float center[] = { 0.f, 0.f, 0.f };
			float normal[] = { 0.f, 1.f, 0.f };
			par_shapes_mesh* mesh = par_shapes_create_disk(12, 12, center, normal);
			App->import->LoadParShape("Disk", mesh);
			par_shapes_free_mesh(mesh);
		}
		//if (ImGui::MenuItem("Tetrahedron"))
		//{
		//	par_shapes_mesh* mesh = par_shapes_create_tetrahedron();
		//	LoadParShape("Tetrahedron", mesh);
		//	par_shapes_free_mesh(mesh);
		//}
		//if (ImGui::MenuItem("Lsystem"))
		//{
		//	par_shapes_mesh* mesh = par_shapes_create_lsystem("String", 10, 10);
		//	LoadParShape("Lsystem", mesh);
		//	par_shapes_free_mesh(mesh);
		//}
		//if (ImGui::MenuItem("Rock"))
		//{
		//	int random_seed = 0;//TODO: Random seed
		//	//With each click it updates
		//	par_shapes_mesh* mesh = par_shapes_create_rock(random_seed, 100);
		//	LoadParShape("Rock", mesh);
		//	par_shapes_free_mesh(mesh);
		//}

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
		LOG("Menu item clicked");
	}
	if (button_clicked)
	{
		//OpenPanel
		LOG("Button clicked");
	}
}