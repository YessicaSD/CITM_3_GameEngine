#include "Mesh.h"
#include "Globals.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

bool Mesh::Load(uint num_vertices, float * mVetices)
{
	this->num_vertices = num_vertices;
	this->vertices = new float[num_vertices * 3];
	memcpy(this->vertices, mVetices, sizeof(float) *num_vertices * 3);
	LOG("New mesh with %d vertices", num_vertices);
	return true;
}

void Mesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, this->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id_indice);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, this->num_indices * 3, GL_UNSIGNED_INT, NULL);
}