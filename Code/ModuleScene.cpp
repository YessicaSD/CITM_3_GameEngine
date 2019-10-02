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
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);	float vertices[3*3] = { 0.f,0.f,0.f, 0.f,0.f,1.f, 1.f,0.f,1.f };	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3 * 3, vertices, GL_STATIC_DRAW);
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

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	glDisableClientState(GL_VERTEX_ARRAY);

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

