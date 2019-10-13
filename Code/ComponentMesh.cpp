#include "ComponentMesh.h"
#include "Globals.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>
#include "AssetMesh.h"

ComponentMesh::ComponentMesh(GameObject * gameobject) : Component(gameobject)
{
}

void ComponentMesh::OnPostUpdate()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indice);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
}

void ComponentMesh::DrawVertexNormal()
{
	glColor3f(0.2f, 1.f, 0.25f);
	uint j = 0;
	float lenght = 2;
	for (uint i = 0; i < mesh->num_vertices*3; i+=3)
	{

		glBegin(GL_LINES);
		glVertex3f(mesh->vertices[i], mesh->vertices[i+1], mesh->vertices[i+2]);
		glVertex3f(mesh->vertices[i] + mesh->normals[j].x*lenght, mesh->vertices[i+1] + mesh->normals[j].y *lenght, mesh->vertices[i+2] + mesh->normals[j].z*lenght);
		++j;
		glEnd();
	}
	glColor3f(1, 1, 1);
}

void ComponentMesh::DrawNormals()
{
	glColor3f(0, 0, 1);
	float lenght = 2;
	for (uint i = 0; i < mesh->numFaces; ++i)
	{
		glBegin(GL_LINES);
		glVertex3f(mesh->face_middle_point[i].x, mesh->face_middle_point[i].y, mesh->face_middle_point[i].z);
		glVertex3f(mesh->face_middle_point[i].x + mesh->faces_normals[i].x*lenght, mesh->face_middle_point[i].y + mesh->faces_normals[i].y *lenght, mesh->face_middle_point[i].z + mesh->faces_normals[i].z*lenght);
		glEnd();
	}
	glColor3f(1, 1, 1);
}
