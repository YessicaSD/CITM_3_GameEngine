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

	panel_cube = App->gui->CreatePanel<PanelCreateShape>("Create cube");
	panel_cube->shape_name = "Cube";
	panel_cube->mesh_function = par_shapes_create_cube;

	panel_parametric_sphere = App->gui->CreatePanel<PanelCreateShape>("Create parametric sphere");
	panel_parametric_sphere->shape_name = "Parametric sphere";
	panel_parametric_sphere->shape_values.push_back({ "Slices", ImGuiDataType_S32, &parametric_sphere_slices });
	panel_parametric_sphere->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &parametric_sphere_stacks });
	panel_parametric_sphere->mesh_function = [&slices = parametric_sphere_slices, &stacks = parametric_sphere_stacks]()
	{ return par_shapes_create_parametric_sphere(slices, stacks); };

	panel_subdivided_sphere = App->gui->CreatePanel<PanelCreateShape>("Create subdivided sphere");
	panel_subdivided_sphere->shape_name = "Subdivided sphere";
	panel_subdivided_sphere->shape_values.push_back({"Number of subdivisions", ImGuiDataType_S32, &subdivided_sphere_nsubdivisions});
	panel_subdivided_sphere->mesh_function = [&nsubdivisions = subdivided_sphere_nsubdivisions]()
	{ return par_shapes_create_subdivided_sphere(nsubdivisions); };

	panel_hemisphere = App->gui->CreatePanel<PanelCreateShape>("Create hemisphere");
	panel_hemisphere->shape_name = "Hemisphere";
	panel_hemisphere->shape_values.push_back({ "Slices", ImGuiDataType_S32, &hemisphere_slices });
	panel_hemisphere->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &hemisphere_stacks });
	panel_hemisphere->mesh_function = [&slices = hemisphere_slices, &stacks = hemisphere_stacks]()
	{ return par_shapes_create_hemisphere(slices, stacks); };

	panel_plane = App->gui->CreatePanel<PanelCreateShape>("Create plane");
	panel_plane->shape_name = "Plane";
	panel_plane->shape_values.push_back({ "Slices", ImGuiDataType_S32, &plane_slices });
	panel_plane->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &plane_stacks });
	panel_plane->mesh_function = [&slices = plane_slices, &stacks = plane_stacks]
	{ return par_shapes_create_plane(slices, stacks); };

	panel_klein_bottle = App->gui->CreatePanel<PanelCreateShape>("Create klein bottle");
	panel_klein_bottle->shape_name = "Klein bottle";
	panel_klein_bottle->shape_values.push_back({ "Slices", ImGuiDataType_S32, &klein_bottle_slices });
	panel_klein_bottle->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &klein_bottle_stacks });
	panel_klein_bottle->mesh_function = [&slices = klein_bottle_slices, &stacks = klein_bottle_stacks]
	{ return par_shapes_create_klein_bottle(slices, stacks); };

	panel_cylinder = App->gui->CreatePanel<PanelCreateShape>("Create cylinder");
	panel_cylinder->shape_name = "Cylinder";
	panel_cylinder->shape_values.push_back({ "Slices", ImGuiDataType_S32, &cylinder_slices });
	panel_cylinder->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &cylinder_stacks });
	panel_cylinder->mesh_function = [&slices = cylinder_slices, &stacks = cylinder_stacks]()
	{ return par_shapes_create_cylinder(slices, stacks); };
	
	panel_cone = App->gui->CreatePanel<PanelCreateShape>("Create cone");
	panel_cone->shape_name = "Cone";
	panel_cone->shape_values.push_back({"Slices", ImGuiDataType_S32, &cone_slices});
	panel_cone->shape_values.push_back({"Stacks", ImGuiDataType_S32, &cone_stacks});
	panel_cone->mesh_function = [&slices = cone_slices, &stacks = cone_stacks]()
	{ return par_shapes_create_cone(slices, stacks); };

	panel_torus = App->gui->CreatePanel<PanelCreateShape>("Create torus");
	panel_torus->shape_name = "Torus";
	panel_torus->shape_values.push_back({ "Slices", ImGuiDataType_S32, &torus_slices });
	panel_torus->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &torus_stacks });
	panel_torus->shape_values.push_back({ "Radius", ImGuiDataType_Float, &torus_radius });
	panel_torus->mesh_function = [&slices = torus_slices, &stacks = torus_stacks, &radius = torus_radius]()
	{ return par_shapes_create_torus(slices, stacks, radius); };

	panel_trefoil_knot = App->gui->CreatePanel<PanelCreateShape>("Create trefoil knot");
	panel_trefoil_knot->shape_name = "Trefoil Knot";
	panel_trefoil_knot->shape_values.push_back({ "Slices", ImGuiDataType_S32, &trefoil_knot_stacks });
	panel_trefoil_knot->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &trefoil_knot_stacks });
	panel_trefoil_knot->shape_values.push_back({ "Radius", ImGuiDataType_Float, &trefoil_knot_radius });
	panel_trefoil_knot->mesh_function = [&slices = trefoil_knot_slices, &stacks = trefoil_knot_stacks, &radius = trefoil_knot_radius]()
	{return par_shapes_create_trefoil_knot(slices, stacks, radius); };

	panel_dodecahedron = App->gui->CreatePanel<PanelCreateShape>("Create dodecahedron");
	panel_dodecahedron->shape_name = "Dodecahedron";
	panel_dodecahedron->mesh_function = par_shapes_create_dodecahedron;

	panel_icosahedron = App->gui->CreatePanel<PanelCreateShape>("Create icosahedron");
	panel_icosahedron->shape_name = "Icosahedron";
	panel_icosahedron->mesh_function = par_shapes_create_icosahedron;

	panel_disk = App->gui->CreatePanel<PanelCreateShape>("Create disk");
	panel_disk->shape_name = "Disk";
	panel_disk->shape_values.push_back({ "Slices", ImGuiDataType_S32, &disk_slices });
	panel_disk->shape_values.push_back({ "Radius", ImGuiDataType_Float, &disk_radius });
	panel_disk->mesh_function = [&radius = disk_radius, &slices = disk_slices, &center = disk_center, &normal = disk_normal]()
	{ return par_shapes_create_disk(radius, slices, center, normal); };

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
			[&slices = parametric_sphere_slices, &stacks = parametric_sphere_stacks]()
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
			[&slices = cylinder_slices, stacks = cylinder_stacks]()
			{ return par_shapes_create_cylinder(slices, stacks); },
			panel_cylinder);

		MenuItem(
			"Cone",
			[&slices = cone_slices, &stacks = cone_stacks]()
			{ return par_shapes_create_cone(slices, stacks); },
			panel_cone);

		MenuItem(
			"Torus",
			[&slices = torus_slices, &stacks = torus_stacks, &radius = torus_radius]()
			{ return par_shapes_create_torus(slices, stacks, radius); },
			panel_torus);

		MenuItem(
			"Trefoil knot",
			[&slices = trefoil_knot_slices, &stacks = trefoil_knot_stacks, &radius = trefoil_knot_radius]()
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
			[&radius = disk_radius, &slices = disk_slices, &center = disk_center, &normal = disk_normal]()
			{ return par_shapes_create_disk(radius, slices, center, normal); },
			panel_disk);

		//par_shapes_create_tetrahedron();
		//par_shapes_create_lsystem();
		//par_shapes_create_rock();

		//TODO: Show menu to configurate this
		ImGui::EndMenu();
	}
}

void MenuCreateShape::MenuItem(std::string name, std::function<par_shapes_mesh*()> mesh_function, Panel * panel)
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
		AssetMesh * asset_mesh = App->import->LoadParShape(mesh);
		par_shapes_free_mesh(mesh);
		App->import->CreateGameObjectWithMesh(name, &App->scene->root_gameobject.transform, asset_mesh);
	}
	if (button_clicked)
	{
		panel->SetActive(true);
		//TODO: Open panel
		//Keep a pointer to the panel you've accessed
		//Draw it every frame the panel is closed
		//This should be done in another function, because this one is going to be executed while creating the main menu bar
	}
}

//TODO: Pass arguments
//Mainly the function to be called
void PanelCreateShape::CreateMultiple()
{
	ImGui::Separator();
	ImGui::Text("Create multiple");
	ImGui::InputInt3  ("Copies    ", copies);
	ImGui::InputFloat3("Separation", separation);

	ImGui::Separator();
}

//Panels code

PanelCreateShape::PanelCreateShape(std::string name, bool active, std::vector<SDL_Scancode> shortcut) :
	Panel(name, active, shortcut)
{}

void PanelCreateShape::Draw()
{
	ImGui::Begin(name.c_str());
	for (std::vector<ShapeValue>::iterator iter = shape_values.begin();
		iter != shape_values.end();
		++iter)
	{
		ImGui::InputScalar((*iter).name.c_str(), (*iter).data_type, (*iter).value_ptr);
	}
	CreateMultiple();
	if (ImGui::Button("Cancel"))
	{
		SetActive(false);
	}
	ImGui::SameLine();
	if (ImGui::Button("Create"))
	{
		par_shapes_mesh* mesh = mesh_function();
		AssetMesh * asset_mesh = App->import->LoadParShape(mesh);
		par_shapes_free_mesh(mesh);
		if (copies[0] > 0 && copies[1] > 0 && copies[2] > 0)
		{
			for (int x = 0; x < copies[0]; ++x)
			{
				for (int y = 0; y < copies[1]; ++y)
				{
					for (int z = 0; z < copies[2]; ++z)
					{
						App->import->CreateGameObjectWithMesh(shape_name, &App->scene->root_gameobject.transform, asset_mesh);
						//TODO: Move gameobjects
					}
				}
			}
		}
		else if (copies[0] > 0 && copies[1] > 0)
		{
			for (int x = 0; x < copies[0]; ++x)
			{
				for (int y = 0; y < copies[1]; ++y)
				{
					App->import->CreateGameObjectWithMesh(shape_name, &App->scene->root_gameobject.transform, asset_mesh);
					//TODO: Move gameobjects
				}
			}
		}
		else if (copies[0] > 0 && copies[2] > 0)
		{
			for (int x = 0; x < copies[0]; ++x)
			{
				for (int z = 0; z < copies[2]; ++z)
				{
					App->import->CreateGameObjectWithMesh(shape_name, &App->scene->root_gameobject.transform, asset_mesh);
					//TODO: Move gameobjects
				}
			}
		}
		else if (copies[1] > 0 && copies[2] > 0)
		{
			for (int y = 0; y < copies[1]; ++y)
			{
				for (int z = 0; z < copies[2]; ++z)
				{
					App->import->CreateGameObjectWithMesh(shape_name, &App->scene->root_gameobject.transform, asset_mesh);
					//TODO: Move gameobjects
				}
			}
		}
		else if (copies[0] > 0)
		{
			for (int x = 0; x < copies[0]; ++x)
			{
				App->import->CreateGameObjectWithMesh(shape_name, &App->scene->root_gameobject.transform, asset_mesh);
				//TODO: Move gameobjects
			}
		}
		else if (copies[1] > 0)
		{
			for (int y = 0; y < copies[1]; ++y)
			{
				App->import->CreateGameObjectWithMesh(shape_name, &App->scene->root_gameobject.transform, asset_mesh);
				//TODO: Move gameobjects
			}
		}
		else if (copies[2] > 0)
		{
			for (int z = 0; z < copies[2]; ++z)
			{
				App->import->CreateGameObjectWithMesh(shape_name, &App->scene->root_gameobject.transform, asset_mesh);
				//TODO: Move gameobjects
			}
		}
		//TODO: Create a vector with the offset and use it
		SetActive(false);
	}
	ImGui::End();
}

ShapeValue::ShapeValue(std::string name, ImGuiDataType data_type, void * value_ptr):
	name(name),
	data_type(data_type),
	value_ptr(value_ptr)
{}
