#include "Cube.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

Cube::Cube(float vertices[NUM_VERTECES * 3], uint indices[NUM_INDICES])
{
	//Opening buffer for vertexs
	glGenBuffers(1, (GLuint*) & (vetex_buf_id));
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_VERTECES * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) & (indices_buf_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NUM_INDICES, indices, GL_STATIC_DRAW);

}

void Cube::Draw_DirectMode()
{
}

void Cube::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf_id);
	glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);
}
