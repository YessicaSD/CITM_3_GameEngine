#include "ComponentMesh.h"
#include "Globals.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

ComponentMesh::ComponentMesh(GameObject * gameobject) : Component(gameobject)
{
}

bool ComponentMesh::LoadMesh(uint num_vertices, float * mVetices)
{
	this->num_vertices = num_vertices;
	this->vertices = new float[num_vertices * 3];
	memcpy(this->vertices, mVetices, sizeof(float) *num_vertices * 3);
	LOG("New mesh with %d vertices", num_vertices);
	return true;
}

void ComponentMesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, this->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id_indice);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_INT, NULL);
}

void ComponentMesh::DrawVertexNormal()
{
	glColor3f(0.2f, 1.f, 0.25f);
	uint j = 0;
	float lenght = 2;
	for (uint i = 0; i < num_vertices*3; i+=3)
	{

		glBegin(GL_LINES);
		glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
		glVertex3f(vertices[i] + normals[j].x*lenght, vertices[i+1] + normals[j].y *lenght, vertices[i+2] + normals[j].z*lenght);
		++j;
		glEnd();
	}
	glColor3f(1, 1, 1);
}

void ComponentMesh::DrawNormals()
{
	glColor3f(0, 0, 1);
	float lenght = 2;
	for (uint i = 0; i < numFaces; ++i)
	{
		glBegin(GL_LINES);
		glVertex3f(face_middle_point[i].x, face_middle_point[i].y, face_middle_point[i].z);
		glVertex3f(face_middle_point[i].x + faces_normals[i].x*lenght, face_middle_point[i].y + faces_normals[i].y *lenght, face_middle_point[i].z + faces_normals[i].z*lenght);
		glEnd();
	}
	glColor3f(1, 1, 1);
}
