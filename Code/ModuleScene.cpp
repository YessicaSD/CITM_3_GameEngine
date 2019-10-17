#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#define PAR_SHAPES_IMPLEMENTATION
#include "par\par_shapes.h"
#include "ComponentMesh.h"
#include "ModuleImport.h"
#include "AssetMesh.h"



ModuleScene::ModuleScene(bool start_enabled) :
	Module(start_enabled),
	root_gameobject("Root", nullptr)
{
	view_mode["default"] = true;
	view_mode["wireframe"] = false;
	view_mode["vertex"] = false;
	view_mode["vertices_normals"] = false;
	view_mode["face_normals"] = false;
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));	

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	return true;
}


// Update: draw background
update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::GameObjectPostUpdateRecursive(ComponentTransform * object)
{
	object->gameobject->OnPostUpdate();
	for(std::vector<ComponentTransform *>::iterator iter = object->children.begin();
		iter != object->children.end();
		++iter)
	{
		GameObjectPostUpdateRecursive((*iter));
	}
}

update_status ModuleScene::PostUpdate()
{
	GameObjectPostUpdateRecursive(&root_gameobject.transform);

	//if (view_mode["default"])
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glColor3f(1, 1, 1);
	//	Draw();
	//}

	//if (view_mode["wireframe"])
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	glColor3f(0, 0, 1);
	//	Draw();
	//}

	//if (view_mode["vertex"])
	//{
	//	glColor3f(1, 0, 0);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//	glPointSize(5);
	//	Draw();
	//}
	//if (view_mode["vertices_normals"])
	//{
	//	glColor3f(0, 1, 0);
	//	glLineWidth(5);
	//	DrawVertexNormals();
	//}
	//if (view_mode["face_normals"])
	//{
	//	glColor3f(0, 1, 0);
	//	glLineWidth(5);
	//	DrawFaceNormals();
	//}

	//glColor3f(1, 1, 1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	PPlane p(0, 1, 0, 0);
	p.axis = true;
	p.wire = false;
	p.Render();

	return UPDATE_CONTINUE;
}

//TODO: Remove this, we shouldn't iterate meshes directly
//We should instead gameobjects
//And if that gameobject has a mesh component, it should draw its mesh
//Maybe if it's on debug, it should draw its normals
//void ModuleScene::DrawFaceNormals()
//{
//	for (std::vector<AssimpScene*>::iterator scene_iter = App->import->array_scene.begin(); scene_iter != App->import->array_scene.end(); ++scene_iter)
//	{
//		for (std::vector<ComponentMesh*>::iterator iter = (*scene_iter)->assimp_meshes.begin(); iter != (*scene_iter)->assimp_meshes.end(); ++iter)
//		{
//			if ((*iter))
//			{
//				(*iter)->DrawNormals();
//			}
//		}
//	}
//}

//void ModuleScene::DrawVertexNormals()
//{
//	for (std::vector<AssimpScene*>::iterator scene_iter = App->import->array_scene.begin(); scene_iter != App->import->array_scene.end(); ++scene_iter)
//	{
//		for (std::vector<ComponentMesh*>::iterator iter = (*scene_iter)->assimp_meshes.begin(); iter != (*scene_iter)->assimp_meshes.end(); ++iter)
//		{
//			if ((*iter))
//			{
//				(*iter)->DrawVertexNormal();
//			}
//		}
//	}
//}

void ModuleScene::ChangeRenderMode(std::string variable)
{
	std::map<std::string, bool>::iterator iter = view_mode.find(variable);
	if (iter != view_mode.end())
	{
		(*iter).second = !(*iter).second;
	}
}

bool ModuleScene::GetRenderMode(std::string variable)
{
	std::map<std::string, bool>::iterator iter = view_mode.find(variable);
	if (iter != view_mode.end())
	{
		return (*iter).second;
	}
	return false;
}

void ModuleScene::CreateMenu()
{
	if (ImGui::BeginMenu("Create"))
	{
		//Create the different par shapes

		//TODO: Align buttons

		if (ImGui::MenuItem("Cube"))
		{
			par_shapes_mesh* mesh = par_shapes_create_cube();
			LoadParShape("Cube", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Parametric sphere"))
		{
			par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(10, 10);
			LoadParShape("Parametric sphere", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Subdivided sphere"))
		{
			par_shapes_mesh* mesh = par_shapes_create_subdivided_sphere(1);
			LoadParShape("Subdivided sphere", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Hemisphere"))
		{
			par_shapes_mesh* mesh = par_shapes_create_hemisphere(50, 20);
			LoadParShape("Hemisphere", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Plane"))
		{
			par_shapes_mesh* mesh = par_shapes_create_plane(10, 10);
			LoadParShape("Plane", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Klein"))
		{
			par_shapes_mesh* mesh = par_shapes_create_klein_bottle(10, 10);
			LoadParShape("Klein", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Cylinder"))
		{
			par_shapes_mesh* mesh = par_shapes_create_cylinder(50,10);
			LoadParShape("Cylinder", mesh);
			par_shapes_free_mesh(mesh);
		}

		//CONE
		//TODO: Find a way to make the size smaller but keep it in the center
		float button_height = 12.5f;
		float space = 150;
		bool selectable_clicked = false;
		bool button_clicked = false;
		ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_Text);
		ImVec2 button_size(button_height, button_height);

		ImGui::Selectable("Cone", false);
		ImGui::SetItemAllowOverlap();
		selectable_clicked = ImGui::IsItemClicked();
		ImGui::SameLine(space);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100.f);
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, color);
		ImVec2 cursor_pos = ImGui::GetCursorPos();
		int text_height = ImGui::GetFontSize() * 1;
		ImGuiStyle style = ImGui::GetStyle();//ImGuiStyleVar_FramePadding
		cursor_pos.y += text_height + style.FramePadding.y * 2;
		ImGui::SetCursorPos(cursor_pos);
		ImGui::Button("", button_size);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		//INFO: We're using ImGui::IsItemClicked() instead of the return of ImGui::Button because they execute on different frames
		button_clicked = ImGui::IsItemClicked();

		if (selectable_clicked && !button_clicked)
		{
			LOG("Selectable clicked");
		}
		if (button_clicked)
		{
			LOG("Button clicked");
		}

		//TODO: Options for creating shapes
		if (ImGui::MenuItem("Torus"))
		{
			par_shapes_mesh* mesh = par_shapes_create_torus(12,12,0.5f);
			LoadParShape("Torus", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Trefoil"))
		{
			par_shapes_mesh* mesh = par_shapes_create_trefoil_knot(50, 20, 2);
			LoadParShape("Trefoil", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Dodecahedron"))
		{
			par_shapes_mesh* mesh = par_shapes_create_dodecahedron();
			LoadParShape("Dodecahedron", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Icosahedron"))
		{
			par_shapes_mesh* mesh = par_shapes_create_icosahedron();
			LoadParShape("Icosahedron", mesh);
			par_shapes_free_mesh(mesh);
		}
		if (ImGui::MenuItem("Disk"))
		{
			float center[] = { 0.f, 0.f, 0.f };
			float normal[] = { 0.f, 1.f, 0.f };
			par_shapes_mesh* mesh = par_shapes_create_disk(12, 12, center, normal);
			LoadParShape("Disk", mesh);
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

void ModuleScene::LoadParShape(std::string name, par_shapes_mesh * mesh)
{
	AssetMesh * asset_mesh = new AssetMesh();
	asset_mesh->LoadVertices(mesh->npoints, mesh->points);
	asset_mesh->LoadFacesAndNormals(mesh->ntriangles, mesh->triangles);
	asset_mesh->GenerateVerticesBuffer();
	asset_mesh->GenerateFacesAndNormalsBuffer();
	App->import->meshes.push_back(asset_mesh);

	GameObject * new_gameobject = new GameObject(name, &root_gameobject.transform);
	ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
	component_mesh->mesh = asset_mesh;
}