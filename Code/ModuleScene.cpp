#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "ModuleRandom.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>


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

	//glGenBuffers(1, (GLuint*) &(my_id));
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	////							//0				1			2				3			4				5				6			7
	//float vertices[8*3] = { 0.f,0.f,0.f,  0.f,0.f,1.f,  1.f,0.f,1.f,    1.f,0.f,0.f,   0.f,1.f,0.f,  0.f,1.f,1.f,  1.f,1.f,1.f,  1.f,1.f,0.f};
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8 * 3, vertices, GL_STATIC_DRAW);

	////6 faces 2 triangle for wach and each triangle is form by 3 vetexs.
	//const uint num_index = 6 * 2 * 3;
	////								Down		Right			Back		  Up		Left			Front
	//uint indices[num_index] = {0,2,1, 0,3,2, 1,2,5,2,6,5 ,2,3,6,3,7,6,   4,5,6,6,7,4,	0,4,3,4,7,3,	0,1,5,5,4,0	};
	//glGenBuffers(1, (GLuint*) &(my_indices));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_index, indices, GL_STATIC_DRAW);
	
	float3 position = { 0,0,0 };
	cube[0]=new Cube(1.f,1.f,1.f, position);
	position = { 2,0,0 };
	cube[1]=new Cube(1.f,1.f,1.f, position);
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
	//glBegin(GL_TRIANGLES);

	const uint num_index = 6 * 2 * 3;


	cube[0]->Draw();
	cube[1]->Draw();

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//// … draw other buffers
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	//glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	//float lenght = 1;
	////Down
	//glVertex3f(0, 0, 0);
	//glVertex3f(1* lenght, 0, 1 * lenght);
	//glVertex3f(0, 0, 1 * lenght);

	//glVertex3f(0, 0, 0);
	//glVertex3f(1 * lenght, 0, 0);
	//glVertex3f(1 * lenght, 0, 1 * lenght);

	////Up
	//glVertex3f(0, 1 * lenght, 0);
	//glVertex3f(0, 1 * lenght, 1 * lenght);
	//glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);

	//glVertex3f(0, 1 * lenght, 0);
	//glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	//glVertex3f(1 * lenght, 1 * lenght, 0);

	////Right
	//glVertex3f(0, 0, 1 * lenght);
	//glVertex3f(1 * lenght, 0, 1 * lenght);
	//glVertex3f(0, 1 * lenght, 1 * lenght);

	//glVertex3f(1 * lenght, 0, 1 * lenght);
	//glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	//glVertex3f(0, 1 * lenght, 1 * lenght);

	////Left
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 1 * lenght, 0);
	//glVertex3f(1 * lenght, 0, 0);

	//glVertex3f(0, 1 * lenght, 0);
	//glVertex3f(1 * lenght, 1 * lenght, 0);
	//glVertex3f(1 * lenght, 0, 0);

	////Front
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 0, 1 * lenght);
	//glVertex3f(0, 1 * lenght, 1 * lenght);
	//
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 1 * lenght, 1 * lenght);
	//glVertex3f(0, 1 * lenght, 0);

	////Back
	//glVertex3f(1 * lenght, 0, 0);
	//glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	//glVertex3f(1 * lenght, 0, 1 * lenght);

	//glVertex3f(1 * lenght, 0, 0);
	//glVertex3f(1 * lenght, 1 * lenght, 0);
	//glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);


	//glEnd();


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

