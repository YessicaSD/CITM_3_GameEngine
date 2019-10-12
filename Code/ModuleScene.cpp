#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#define PAR_SHAPES_IMPLEMENTATION
#include "par\par_shapes.h"
#include "Mesh.h"
#include "ModuleImport.h"



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
	
	//App->importFBX->LoadMesh("Assets/BakerHouse.FBX");

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
	/*if (view_mode["default"])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		cube[1]->Draw();

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glDrawElements(GL_TRIANGLES, sphereInfo->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	}	
	
	if (view_mode["wireframe"])
	{
		glColor3f(0, 0, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cube[1]->Draw();

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glDrawElements(GL_TRIANGLES, sphereInfo->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);

		glColor3f(1, 1, 1);
	}

	if (view_mode["vertex"])
	{
		glColor3f(1, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(3);
		cube[1]->Draw();

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glDrawElements(GL_TRIANGLES, sphereInfo->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
		glColor3f(1, 1, 1);
	}
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate()
{
	if (view_mode["default"])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1, 1, 1);
		Draw();
	}

	if (view_mode["wireframe"])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0, 0, 1);
		Draw();
	}

	if (view_mode["vertex"])
	{
		glColor3f(1, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(5);
		Draw();
	}
	if (view_mode["vertices_normals"])
	{
		glColor3f(0, 1, 0);
		glLineWidth(5);
		DrawVertexNormals();
	}
	if (view_mode["face_normals"])
	{
		glColor3f(0, 1, 0);
		glLineWidth(5);
		DrawFaceNormals();
	}

	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	PPlane p(0, 1, 0, 0);
	p.axis = true;
	p.wire = false;
	p.Render();



	return UPDATE_CONTINUE;
}

void ModuleScene::DrawFaceNormals()
{
	for (std::vector<AssimpScene*>::iterator scene_iter = App->import->array_scene.begin(); scene_iter != App->import->array_scene.end(); ++scene_iter)
	{
		for (std::vector<Mesh*>::iterator iter = (*scene_iter)->assimp_meshes.begin(); iter != (*scene_iter)->assimp_meshes.end(); ++iter)
		{
			if ((*iter))
			{
				(*iter)->DrawNormals();
			}
		}
	}
}

void ModuleScene::DrawVertexNormals()
{
	for (std::vector<AssimpScene*>::iterator scene_iter = App->import->array_scene.begin(); scene_iter != App->import->array_scene.end(); ++scene_iter)
	{
		for (std::vector<Mesh*>::iterator iter = (*scene_iter)->assimp_meshes.begin(); iter != (*scene_iter)->assimp_meshes.end(); ++iter)
		{
			if ((*iter))
			{
				(*iter)->DrawVertexNormal();
			}
		}
	}
}

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

void ModuleScene::Draw()
{
	for (std::vector<AssimpScene*>::iterator scene_iter = App->import->array_scene.begin(); scene_iter != App->import->array_scene.end(); ++scene_iter)
	{
		for (std::vector<Mesh*>::iterator iter = (*scene_iter)->assimp_meshes.begin(); iter != (*scene_iter)->assimp_meshes.end(); ++iter)
		{
			if ((*iter))
			{
				(*iter)->Draw();
			}
		}
	}
}

void ModuleScene::CreateMenu()
{
	if (ImGui::BeginMenu("Create"))
	{
		//Create the different par shapes

		if (ImGui::Button("Cube"))
		{

		}
		if (ImGui::Button("Sphere"))
		{

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
			//TODO: Create a gameobject with a mesh component like this

			//TODO: Show menu to configurate this

			par_shapes_mesh* mesh = par_shapes_create_trefoil_knot(50, 20, 2);
			uint mesh_id = 0u;
			uint mesh_indices_id = 0u;

			glGenBuffers(1, &mesh_id);
			glBindBuffer(GL_ARRAY_BUFFER, mesh_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->npoints * 3, mesh->points, GL_STATIC_DRAW);
			
			glGenBuffers(1, &mesh_indices_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_indices_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * mesh->ntriangles * 3, mesh->triangles, GL_STATIC_DRAW);
			
			par_shapes_free_mesh(mesh);
		}
		ImGui::EndMenu();
	}
}