#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#define PAR_SHAPES_IMPLEMENTATION
#include "par\par_shapes.h"


ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
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

	float3 position = { 0,0,0 };
	cube[0]=new Cube(1.f,1.f,1.f, position);
	position = { 2,0,0 };
	cube[1]=new Cube(1.f,1.f,1.f, position);

	sphereInfo = par_shapes_create_subdivided_sphere(1);
	sphere_v_id = 0;
	glGenBuffers(1, &sphere_v_id);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphereInfo->npoints * 3, sphereInfo->points, GL_STATIC_DRAW);

	glGenBuffers(1, &sphere_indice_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T)*sphereInfo->ntriangles * 3, sphereInfo->triangles, GL_STATIC_DRAW);
	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	par_shapes_free_mesh(sphereInfo);
	return true;
}

// Update: draw background
update_status ModuleScene::Update(float dt)
{
	//glBegin(GL_TRIANGLES);

	const uint num_index = 6 * 2 * 3;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	cube[0]->Draw_DirectMode();
	cube[1]->Draw();

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, sphereInfo->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate()
{
	PPlane p(0, 1, 0, 0);
	p.axis = true;
	p.wire = false;
	p.Render();
	return UPDATE_CONTINUE;
}

