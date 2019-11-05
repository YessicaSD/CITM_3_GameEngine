#include "MenuCreateShape.h"
#include "par\par_shapes.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleImport.h"
#include "Globals.h"
#include "AssetMesh.h"

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

	PanelCreateShape * panel_cube = App->gui->CreatePanel<PanelCreateShape>("cube");
	panel_cube->mesh_function = par_shapes_create_cube;
	panels_create_shape.push_back(panel_cube);

	PanelCreateShape* panel_parametric_sphere = App->gui->CreatePanel<PanelCreateShape>("parametric sphere");
	panel_parametric_sphere->shape_values.push_back({ "Slices", ImGuiDataType_S32, &parametric_sphere_slices });
	panel_parametric_sphere->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &parametric_sphere_stacks });
	panel_parametric_sphere->mesh_function = [&slices = parametric_sphere_slices, &stacks = parametric_sphere_stacks]()
	{ return par_shapes_create_parametric_sphere(slices, stacks); };
	panels_create_shape.push_back(panel_parametric_sphere);

	PanelCreateShape* panel_subdivided_sphere = App->gui->CreatePanel<PanelCreateShape>("subdivided sphere");
	panel_subdivided_sphere->shape_values.push_back({"Number of subdivisions", ImGuiDataType_S32, &subdivided_sphere_nsubdivisions});
	panel_subdivided_sphere->mesh_function = [&nsubdivisions = subdivided_sphere_nsubdivisions]()
	{ return par_shapes_create_subdivided_sphere(nsubdivisions); };
	panels_create_shape.push_back(panel_subdivided_sphere);

	PanelCreateShape* panel_hemisphere = App->gui->CreatePanel<PanelCreateShape>("hemisphere");
	panel_hemisphere->shape_values.push_back({ "Slices", ImGuiDataType_S32, &hemisphere_slices });
	panel_hemisphere->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &hemisphere_stacks });
	panel_hemisphere->mesh_function = [&slices = hemisphere_slices, &stacks = hemisphere_stacks]()
	{ return par_shapes_create_hemisphere(slices, stacks); };
	panels_create_shape.push_back(panel_hemisphere);

	PanelCreateShape* panel_plane = App->gui->CreatePanel<PanelCreateShape>("plane");
	panel_plane->shape_values.push_back({ "Slices", ImGuiDataType_S32, &plane_slices });
	panel_plane->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &plane_stacks });
	panel_plane->mesh_function = [&slices = plane_slices, &stacks = plane_stacks]
	{ return par_shapes_create_plane(slices, stacks); };
	panels_create_shape.push_back(panel_plane);

	PanelCreateShape* panel_klein_bottle = App->gui->CreatePanel<PanelCreateShape>("klein bottle");
	panel_klein_bottle->shape_values.push_back({ "Slices", ImGuiDataType_S32, &klein_bottle_slices });
	panel_klein_bottle->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &klein_bottle_stacks });
	panel_klein_bottle->mesh_function = [&slices = klein_bottle_slices, &stacks = klein_bottle_stacks]
	{ return par_shapes_create_klein_bottle(slices, stacks); };
	panels_create_shape.push_back(panel_klein_bottle);

	PanelCreateShape* panel_cylinder = App->gui->CreatePanel<PanelCreateShape>("cylinder");
	panel_cylinder->shape_values.push_back({ "Slices", ImGuiDataType_S32, &cylinder_slices });
	panel_cylinder->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &cylinder_stacks });
	panel_cylinder->mesh_function = [&slices = cylinder_slices, &stacks = cylinder_stacks]()
	{ return par_shapes_create_cylinder(slices, stacks); };
	panels_create_shape.push_back(panel_cylinder);
	
	PanelCreateShape* panel_cone = App->gui->CreatePanel<PanelCreateShape>("cone");
	panel_cone->shape_values.push_back({"Slices", ImGuiDataType_S32, &cone_slices});
	panel_cone->shape_values.push_back({"Stacks", ImGuiDataType_S32, &cone_stacks});
	panel_cone->mesh_function = [&slices = cone_slices, &stacks = cone_stacks]()
	{ return par_shapes_create_cone(slices, stacks); };
	panels_create_shape.push_back(panel_cone);

	PanelCreateShape* panel_torus = App->gui->CreatePanel<PanelCreateShape>("torus");
	panel_torus->shape_values.push_back({ "Slices", ImGuiDataType_S32, &torus_slices });
	panel_torus->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &torus_stacks });
	panel_torus->shape_values.push_back({ "Radius", ImGuiDataType_Float, &torus_radius });
	panel_torus->mesh_function = [&slices = torus_slices, &stacks = torus_stacks, &radius = torus_radius]()
	{ return par_shapes_create_torus(slices, stacks, radius); };
	panels_create_shape.push_back(panel_torus);

	PanelCreateShape* panel_trefoil_knot = App->gui->CreatePanel<PanelCreateShape>("trefoil knot");
	panel_trefoil_knot->shape_values.push_back({ "Slices", ImGuiDataType_S32, &trefoil_knot_stacks });
	panel_trefoil_knot->shape_values.push_back({ "Stacks", ImGuiDataType_S32, &trefoil_knot_stacks });
	panel_trefoil_knot->shape_values.push_back({ "Radius", ImGuiDataType_Float, &trefoil_knot_radius });
	panel_trefoil_knot->mesh_function = [&slices = trefoil_knot_slices, &stacks = trefoil_knot_stacks, &radius = trefoil_knot_radius]()
	{return par_shapes_create_trefoil_knot(slices, stacks, radius); };
	panels_create_shape.push_back(panel_trefoil_knot);

	PanelCreateShape* panel_dodecahedron = App->gui->CreatePanel<PanelCreateShape>("dodecahedron");
	panel_dodecahedron->mesh_function = par_shapes_create_dodecahedron;
	panels_create_shape.push_back(panel_dodecahedron);

	PanelCreateShape* panel_icosahedron = App->gui->CreatePanel<PanelCreateShape>("icosahedron");
	panel_icosahedron->mesh_function = par_shapes_create_icosahedron;
	panels_create_shape.push_back(panel_icosahedron);

	PanelCreateShape* panel_disk = App->gui->CreatePanel<PanelCreateShape>("disk");
	panel_disk->shape_values.push_back({ "Slices", ImGuiDataType_S32, &disk_slices });
	panel_disk->shape_values.push_back({ "Radius", ImGuiDataType_Float, &disk_radius });
	panel_disk->mesh_function = [&radius = disk_radius, &slices = disk_slices, &center = disk_center, &normal = disk_normal]()
	{ return par_shapes_create_disk(radius, slices, center, normal); };
	panels_create_shape.push_back(panel_disk);

	//par_shapes_create_tetrahedron();
	//par_shapes_create_lsystem();
	//par_shapes_create_rock();

	preview_shapes_fbo.GenerateFrameBuffer();
}
MenuCreateShape::~MenuCreateShape()
{
	//if (disk_center)
	//{
	//	delete [] disk_center;
	//	disk_center = nullptr;
	//}
	//if (disk_normal)
	//{
	//	delete[] disk_normal;
	//	disk_normal = nullptr;
	//}
	//
};

void MenuCreateShape::MenuBarTab()
{
	//TODO: Put in constructor
	button_color = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_Text);

	if (ImGui::BeginMenu("Create"))
	{
		for (std::vector<PanelCreateShape*>::iterator iter = panels_create_shape.begin();
			iter != panels_create_shape.end();
			++iter)
		{
			(*iter)->MenuItem(button_height, button_space, button_color, button_size);
		}
		ImGui::EndMenu();
	}
}

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
	shape_name(name),
	Panel("Create " + name, active, shortcut)
{
}

void PanelCreateShape::Draw()
{
	if (App->gui->create_menu->preview_shape_gameobject == nullptr)
	{
		par_shapes_mesh* mesh = mesh_function();
		App->gui->create_menu->preview_shape_mesh = App->import->LoadParShapeMesh(mesh);
		par_shapes_free_mesh(mesh);
		App->gui->create_menu->preview_shape_gameobject = App->import->CreateGameObjectWithMesh("Preview Shape", nullptr, App->gui->create_menu->preview_shape_mesh);
	}

	App->gui->create_menu->preview_shapes_fbo.StartRender(*App->gui->create_menu->preview_shapes_fbo.panel_size);
	App->scene->GameObjectPostUpdateRecursive(App->gui->create_menu->preview_shape_gameobject->transform);
	App->gui->create_menu->preview_shapes_fbo.EndRender();


	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin(name.c_str());
	ImGui::PopStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding);

	//Determine panel size
	//*App->gui->create_menu->preview_shapes_fbo.panel_size = ImGui::GetContentRegionAvail();
	*App->gui->create_menu->preview_shapes_fbo.panel_size = ImVec2(200, 200);
	ImVec2 min = ImGui::GetCursorScreenPos();
	ImVec2 max = ImVec2(min.x + App->gui->create_menu->preview_shapes_fbo.panel_size->x, min.y + App->gui->create_menu->preview_shapes_fbo.panel_size->y);

	ImGui::Image((ImTextureID)App->gui->create_menu->preview_shapes_fbo.render_texture, ImVec2(App->gui->create_menu->preview_shapes_fbo.panel_size->x, App->gui->create_menu->preview_shapes_fbo.panel_size->y), ImVec2(0, 1), ImVec2(1, 0));
	//-------------------------

	bool changed_values = false;
	for (std::vector<ShapeValue>::iterator iter = shape_values.begin();
		iter != shape_values.end();
		++iter)
	{
		
		if (ImGui::InputScalar((*iter).name.c_str(), (*iter).data_type, (*iter).value_ptr)
			&& !changed_values)
		{
			changed_values = true;
		}
	}

	if (changed_values)
	{
		//Delete shape
		RELEASE(App->gui->create_menu->preview_shape_gameobject);
		RELEASE(App->gui->create_menu->preview_shape_mesh);
		//Create new shape with the new values
		par_shapes_mesh* mesh = mesh_function();
		App->gui->create_menu->preview_shape_mesh = App->import->LoadParShapeMesh(mesh);
		par_shapes_free_mesh(mesh);
		App->gui->create_menu->preview_shape_gameobject = App->import->CreateGameObjectWithMesh("Preview Shape", nullptr, App->gui->create_menu->preview_shape_mesh);
	}

	CreateMultiple();
	if (ImGui::Button("Cancel"))
	{
		RELEASE(App->gui->create_menu->preview_shape_gameobject);
		RELEASE(App->gui->create_menu->preview_shape_mesh);
		SetActive(false);
	}
	ImGui::SameLine();
	if (ImGui::Button("Create"))
	{
		App->import->AddMesh(App->gui->create_menu->preview_shape_mesh);
		float3 position = {0.f, 0.f, 0.f};
		for (int x = 0; x < copies[0]; ++x)
		{
			position.y = 0;
			for (int y = 0; y < copies[1]; ++y)
			{
				position.z = 0;
				for (int z = 0; z < copies[2]; ++z)
				{
					GameObject * obj = App->import->CreateGameObjectWithMesh(shape_name, App->scene->root_gameobject->transform, App->gui->create_menu->preview_shape_mesh);
					obj->transform->SetPosition(position);
					if (copies[2] != 0)
					{
						position.z += separation[2];
					}
				}
				if (copies[1] != 0)
				{
					position.y += separation[1];
				}
			}
			if (copies[0] != 0)
			{
				position.x += separation[0];
			}
		}
		SetActive(false);
	}
	ImGui::End();
}

void PanelCreateShape::MenuItem(const float button_height, const float button_space, const ImVec4 &button_color, const ImVec2 &button_size)
{
	ImGui::MenuItem(shape_name.c_str());
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
	ImGui::Button((std::string("##") + shape_name).c_str(), button_size);
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	//INFO: We're using ImGui::IsItemClicked() instead of the return of ImGui::Button because they execute on different frames
	bool button_clicked = ImGui::IsItemClicked();

	if (selectable_clicked && !button_clicked)
	{
		par_shapes_mesh* mesh = mesh_function();
		AssetMesh* asset_mesh = App->import->LoadParShapeMesh(mesh);
		par_shapes_free_mesh(mesh);
		App->import->AddMesh(asset_mesh);
		App->import->CreateGameObjectWithMesh(shape_name, App->scene->root_gameobject->transform, asset_mesh);

	}
	if (button_clicked)
	{
		SetActive(true);
	}
}

ShapeValue::ShapeValue(std::string name, ImGuiDataType data_type, void * value_ptr):
	name(name),
	data_type(data_type),
	value_ptr(value_ptr)
{}
