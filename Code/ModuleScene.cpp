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

	/*float3 position = { 0,0,0 };
	cube[0]=new Cube(1.f,1.f,1.f, position);
	position = { 2,0,0 };
	cube[1]=new Cube(1.f,1.f,1.f, position);

	sphereInfo = par_shapes_create_trefoil_knot(50,20,2);
	glGenBuffers(1, &sphere_v_id);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphereInfo->npoints * 3, sphereInfo->points, GL_STATIC_DRAW);

	glGenBuffers(1, &sphere_indice_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*sphereInfo->ntriangles * 3, sphereInfo->triangles, GL_STATIC_DRAW);*/
	
	//App->import->LoadMesh("Assets/test_childs_2.FBX");

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	//par_shapes_free_mesh(sphereInfo);
	return true;
}


// Update: draw background
update_status ModuleScene::Update(float dt)
{
	//for (std::vector<AssimpScene*>::iterator scene_iter = App->import->array_scene.begin(); scene_iter != App->import->array_scene.end(); ++scene_iter)
	//{
	//	for (std::vector<ComponentMesh*>::iterator iter = (*scene_iter)->assimp_meshes.begin(); iter != (*scene_iter)->assimp_meshes.end(); ++iter)
	//	{
	//		if ((*iter))
	//		{
	//			(*iter)->Draw();
	//		}
	//	}
	//}

	return UPDATE_CONTINUE;
}

void ModuleScene::RecursivePostUpdate(ComponentTransform * object)
{
	object->gameobject->OnPostUpdate();
	for(std::vector<ComponentTransform *>::iterator iter = object->children.begin();
		iter != object->children.end();
		++iter)
	{
		RecursivePostUpdate((*iter));
	}
}

update_status ModuleScene::PostUpdate()
{
	RecursivePostUpdate(&root_gameobject.transform);

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

		if (ImGui::Button("Cube"))
		{
			GameObject * new_gameobject = new GameObject("Cube", &App->scene->root_gameobject.transform);
			ComponentMesh * new_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			//Create cube
			AssetMesh cube_mesh;
			
		}
		if (ImGui::Button("Sphere"))
		{
			GameObject * new_gameobject = new GameObject("Cube", &App->scene->root_gameobject.transform);
			ComponentMesh * new_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			AssetMesh sphere_mesh;

		}
		if (ImGui::Button("Hemisphere"))
		{

		}
		if (ImGui::Button("Plane"))
		{

		}
		if (ImGui::Button("Klein"))
		{

		}
		if (ImGui::Button("Cylinder"))
		{

		}
		if (ImGui::Button("Cone"))
		{

		}
		if (ImGui::Button("Torus"))
		{

		}
		if (ImGui::Button("Trefoil"))
		{
			//Create mesh
			par_shapes_mesh* mesh = par_shapes_create_trefoil_knot(50, 20, 2);

			//Load everything from par_shapes mesh to asset_mesh
			AssetMesh * asset_mesh = new AssetMesh();
			//Vertices
			asset_mesh->num_vertices = mesh->npoints;
			asset_mesh->vertices = mesh->points;

			//Indices
			asset_mesh->num_indices = mesh->ntriangles;
			asset_mesh->indices = (uint *)mesh->triangles;

			par_shapes_free_mesh(mesh);

			
			glGenBuffers(1, &asset_mesh->id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, asset_mesh->id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * asset_mesh->num_vertices* 3, asset_mesh->vertices, GL_STATIC_DRAW);

			//Index

			glGenBuffers(1, &asset_mesh->id_indice);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, asset_mesh->id_indice);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * asset_mesh->num_indices * 3, asset_mesh->indices, GL_STATIC_DRAW);

			//TODO: Add meshes to mesh array in ModuleImport

			//Create gameobject
			GameObject * new_gameobject = new GameObject("Trefoil", &root_gameobject.transform);
			ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			component_mesh->mesh = asset_mesh;


			//TODO: Show menu to configurate this

			//par_shapes_mesh* mesh = par_shapes_create_trefoil_knot(50, 20, 2);
			//uint mesh_id = 0u;
			//uint mesh_indices_id = 0u;

			//glGenBuffers(1, &mesh_id);
			//glBindBuffer(GL_ARRAY_BUFFER, mesh_id);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->npoints * 3, mesh->points, GL_STATIC_DRAW);
			//
			//glGenBuffers(1, &mesh_indices_id);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_indices_id);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * mesh->ntriangles * 3, mesh->triangles, GL_STATIC_DRAW);
			
			//par_shapes_free_mesh(mesh);
		}
		ImGui::EndMenu();
	}
}